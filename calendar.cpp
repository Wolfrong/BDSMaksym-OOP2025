#include "calendar.h"
#include "activity.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <vector>
#include <iostream>

static bool fileExists(const std::string& p)
{
    std::ifstream f(p);
    return f.good();
}

static void readAll(const std::string& p, std::string& out)
{
    std::ifstream is(p, std::ios::binary);
    std::ostringstream ss;
    ss << is.rdbuf();
    out = ss.str();
}

static bool writeAll(const std::string& p, const std::string& s)
{
    std::ofstream os(p, std::ios::binary | std::ios::trunc);
    if (!os) return false;
    os.write(s.data(), (std::streamsize)s.size());
    return (bool)os;
}

std::string CalendarHealth::trim(const std::string& s)
{
    std::size_t a = 0, b = s.size();
    while (a < b && (unsigned char)s[a] <= ' ') ++a;
    while (b > a && (unsigned char)s[b - 1] <= ' ') --b;
    return s.substr(a, b - a);
}

bool CalendarHealth::startsWith(const std::string& s, const std::string& p)
{
    return s.size() >= p.size() &&
           std::equal(p.begin(), p.end(), s.begin());
}

std::string CalendarHealth::today_ddmmyyyy()
{
    std::time_t t = std::time(nullptr);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char buf[11];
    std::strftime(buf, sizeof(buf), "%d-%m-%Y", &tm);
    return std::string(buf);
}

CalendarHealth::CalendarHealth(std::string path)
    : path_(std::move(path))
{
    ensureSkeleton();
}

bool CalendarHealth::ensureSkeleton() const
{
    if (fileExists(path_)) return true;

    const char* skel =
        "[activity]\n\n"
        "[food]\n\n"
        "[calories]\n\n"
        "[mood]\n";

    return writeAll(path_, skel);
}

bool CalendarHealth::insertUnderSection(const std::string& section,
                                        const std::string& line) const
{
    if (!ensureSkeleton())
        return false;
    std::ifstream in(path_);
    if (!in) return false;

    std::vector<std::string> lines;
    std::string cur;
    while (std::getline(in, cur))
    {
        lines.push_back(cur);
    }
    in.close();

    int secIndex = -1;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (lines[i] == section) {
            secIndex = static_cast<int>(i);
            break;
        }
    }

    if (secIndex == -1)
    {
        if (!lines.empty() && !lines.back().empty())
            lines.push_back("");
        lines.push_back(section);
        secIndex = static_cast<int>(lines.size()) - 1;
    }

    std::size_t insertPos = static_cast<std::size_t>(secIndex + 1);
    while (insertPos < lines.size()
           && !( !lines[insertPos].empty() && lines[insertPos][0] == '[' ))
    {
        ++insertPos;
    }

    lines.insert(lines.begin() + static_cast<long>(insertPos), line);
    std::ofstream out(path_, std::ios::binary | std::ios::trunc);
    if (!out) return false;
    for (const auto& l : lines)
    {
        out << l << "\n";
    }
    return (bool)out;
}

bool CalendarHealth::addActivity(const std::string& date,
                                 const std::string& type,
                                 double durationMin,
                                 double weightKg)
{
    std::ostringstream ln;
    ln.setf(std::ios::fixed);
    ln.precision(2);
    ln << date << ' ' << type << ' ' << durationMin << ' ' << weightKg;
    return insertUnderSection("[activity]", ln.str());
}

bool CalendarHealth::addFood(const std::string& date,
                             const std::string& dish,
                             double grams,
                             double kcal,
                             double protein,
                             double fat,
                             double carbs,
                             double waterLiters)
{
    std::ostringstream ln;
    ln.setf(std::ios::fixed);
    ln.precision(2);
    ln << date << ' ' << dish
       << " grams=" << grams
       << " kcal="  << kcal
       << " P="     << protein
       << " F="     << fat
       << " C="     << carbs
       << " waterL="<< waterLiters;
    return insertUnderSection("[food]", ln.str());
}

bool CalendarHealth::addCalories(const std::string& date,
                                 const std::string& dish,
                                 int calories)
{
    return addFood(date, dish, 0.0, (double)calories,
                   0.0, 0.0, 0.0, 0.0);
}

bool CalendarHealth::addMood(const std::string& date,
                             int mood,
                             const std::string& note)
{
    std::ostringstream ln;
    ln << date << ' ' << mood << ' ' << note;
    return insertUnderSection("[mood]", ln.str());
}

bool CalendarHealth::computeCaloriesForDate(const std::string& date,
                                            double& kcalIn,
                                            double& kcalOut) const
{
    kcalIn  = 0.0;
    kcalOut = 0.0;
    std::ifstream in(path_);
    if (!in) return false;
    std::string line, curSec;
    while (std::getline(in, line))
    {
        std::string s = trim(line);
        if (s.empty()) continue;
        if (s.front() == '[' && s.back() == ']')
        {
            curSec = s;
            continue;
        }
        if (s.size() < 10) continue;
        if (!startsWith(s, date)) continue;
        std::string payload = (s.size() > 11 ? s.substr(11) : std::string());

        if (curSec == "[food]") {
            auto pos = payload.find("kcal=");
            if (pos != std::string::npos) {
                pos += 5;
                std::size_t end = pos;
                while (end < payload.size()
                       && (std::isdigit((unsigned char)payload[end]) ||
                           payload[end] == '.'))
                    ++end;
                try {
                    double kcal = std::stod(payload.substr(pos, end - pos));
                    kcalIn += kcal;
                } catch (...) { }
            }
        }
        else if (curSec == "[activity]") {
            std::istringstream ss(payload);
            std::string name; double duration = 0.0, weight = 0.0;
            if (ss >> name >> duration >> weight) {
                ActivityInfo info = ActivityInfo::find_activity(name);
                double burned = duration * info.MET * weight * 0.0175;
                kcalOut += burned;
            }
        }
    }
    return true;
}

bool CalendarHealth::writeCaloriesSummary(const std::string& date)
{
    double inK  = 0.0, outK = 0.0;
    if (!computeCaloriesForDate(date, inK, outK)) return false;
    std::ostringstream ln;
    ln.setf(std::ios::fixed);
    ln << std::setprecision(2);
    ln << date
       << " in="  << inK
       << " out=" << outK;

    return insertUnderSection("[calories]", ln.str());
}

bool CalendarHealth::listByDate(const std::string& date,
                                std::vector<std::string>& out) const
{
    out.clear();
    std::ifstream in(path_);
    if (!in) return false;
    std::string line, curSec;
    while (std::getline(in, line))
    {
        std::string s = trim(line);
        if (s.empty()) continue;
        if (s.front() == '[' && s.back() == ']') {
            curSec = s;
            continue;
        }
        if (s.size() < 10) continue;

        if (startsWith(s, date))
        {
            if (curSec == "[activity]")
                out.push_back("[activity] " + s.substr(11));
            else if (curSec == "[food]")
                out.push_back("[food]     " + s.substr(11));
            else if (curSec == "[calories]")
                out.push_back("[calories] " + s.substr(11));
            else if (curSec == "[mood]")
                out.push_back("[mood]     " + s.substr(11));
            else
                out.push_back(s);
        }
    }
    return true;
}

CalendarDecorator::CalendarDecorator(CalendarBase* inner)
    : inner_(inner)
{
}

CalendarDecorator::~CalendarDecorator()
{
    delete inner_;
    inner_ = nullptr;
}

bool CalendarDecorator::addActivity(const std::string& date,
                                    const std::string& type,
                                    double durationMin,
                                    double weightKg)
{
    return inner_ ? inner_->addActivity(date, type, durationMin, weightKg)
                  : false;
}

bool CalendarDecorator::addFood(const std::string& date,
                                const std::string& dish,
                                double grams,
                                double kcal,
                                double protein,
                                double fat,
                                double carbs,
                                double waterLiters)
{
    return inner_ ? inner_->addFood(date, dish, grams, kcal,
                                    protein, fat, carbs, waterLiters)
                  : false;
}

bool CalendarDecorator::addCalories(const std::string& date,
                                    const std::string& dish,
                                    int calories)
{
    return inner_ ? inner_->addCalories(date, dish, calories)
                  : false;
}

bool CalendarDecorator::addMood(const std::string& date,
                                int mood,
                                const std::string& note)
{
    return inner_ ? inner_->addMood(date, mood, note)
                  : false;
}

bool CalendarDecorator::writeCaloriesSummary(const std::string& date)
{
    return inner_ ? inner_->writeCaloriesSummary(date) : false;
}

bool CalendarDecorator::listByDate(const std::string& date,
                                   std::vector<std::string>& out) const
{
    return inner_ ? inner_->listByDate(date, out) : false;
}

LoggingCalendarDecorator::LoggingCalendarDecorator(CalendarBase* inner)
    : CalendarDecorator(inner)
{
}

bool LoggingCalendarDecorator::addActivity(const std::string& date,
                                           const std::string& type,
                                           double durationMin,
                                           double weightKg)
{
    std::cout << "[Calendar LOG] addActivity: date=" << date
              << " type=" << type
              << " durationMin=" << durationMin
              << " weightKg=" << weightKg << "\n";
    return CalendarDecorator::addActivity(date, type, durationMin, weightKg);
}

bool LoggingCalendarDecorator::addFood(const std::string& date,
                                       const std::string& dish,
                                       double grams,
                                       double kcal,
                                       double protein,
                                       double fat,
                                       double carbs,
                                       double waterLiters)
{
    std::cout << "[Calendar LOG] addFood: date=" << date
              << " dish=" << dish
              << " grams=" << grams
              << " kcal=" << kcal << "\n";
    return CalendarDecorator::addFood(date, dish, grams, kcal,
                                      protein, fat, carbs, waterLiters);
}

bool LoggingCalendarDecorator::addMood(const std::string& date,
                                       int mood,
                                       const std::string& note)
{
    std::cout << "[Calendar LOG] addMood: date=" << date
              << " mood=" << mood
              << " note=" << note << "\n";
    return CalendarDecorator::addMood(date, mood, note);
}

bool LoggingCalendarDecorator::writeCaloriesSummary(const std::string& date)
{
    std::cout << "[Calendar LOG] writeCaloriesSummary: date=" << date << "\n";
    return CalendarDecorator::writeCaloriesSummary(date);
}

bool LoggingCalendarDecorator::listByDate(const std::string& date,
                                          std::vector<std::string>& out) const
{
    std::cout << "[Calendar LOG] listByDate: date=" << date << "\n";
    return CalendarDecorator::listByDate(date, out);
}

ValidatingCalendarDecorator::ValidatingCalendarDecorator(CalendarBase* inner)
    : CalendarDecorator(inner)
{
}

bool ValidatingCalendarDecorator::addActivity(const std::string& date,
                                              const std::string& type,
                                              double durationMin,
                                              double weightKg)
{
    if (durationMin <= 0.0 || weightKg <= 0.0) {
        std::cout << "[Calendar VALIDATION] invalid activity: duration/weight must be > 0\n";
        return false;
    }
    return CalendarDecorator::addActivity(date, type, durationMin, weightKg);
}

bool ValidatingCalendarDecorator::addFood(const std::string& date,
                                          const std::string& dish,
                                          double grams,
                                          double kcal,
                                          double protein,
                                          double fat,
                                          double carbs,
                                          double waterLiters)
{
    if (grams <= 0.0) {
        std::cout << "[Calendar VALIDATION] invalid food: grams must be > 0\n";
        return false;
    }
    if (waterLiters < 0.0) {
        std::cout << "[Calendar VALIDATION] invalid food: waterLiters must be >= 0\n";
        return false;
    }
    return CalendarDecorator::addFood(date, dish, grams, kcal,
                                      protein, fat, carbs, waterLiters);
}

bool ValidatingCalendarDecorator::addMood(const std::string& date,
                                          int mood,
                                          const std::string& note)
{
    if (mood < 1 || mood > 5) {
        std::cout << "[Calendar VALIDATION] invalid mood: must be in [1..5]\n";
        return false;
    }
    return CalendarDecorator::addMood(date, mood, note);
}
