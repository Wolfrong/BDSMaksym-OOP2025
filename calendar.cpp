#include "calendar.h"
#include "activity.h"   // для ActivityInfo::find_activity
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <cctype>

static bool fileExists(const std::string& p)
{
    std::ifstream f(p); return f.good();
}

static void readAll(const std::string& p, std::string& out)
{
    std::ifstream is(p, std::ios::binary);
    std::ostringstream ss; ss << is.rdbuf(); out = ss.str();
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
    while (b > a && (unsigned char)s[b-1] <= ' ') --b;
    return s.substr(a, b-a);
}

bool CalendarHealth::startsWith(const std::string& s, const std::string& p)
{
    return s.size() >= p.size() && std::equal(p.begin(), p.end(), s.begin());
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

bool CalendarHealth::insertUnderSection(const std::string& section, const std::string& line) const
{
    std::string text; readAll(path_, text);
    if (text.empty()) { if (!ensureSkeleton()) return false; readAll(path_, text); }

    std::size_t pos = text.find(section);
    if (pos == std::string::npos)
    {
        text += (text.size() && text.back()!='\n') ? "\n" : "";
        text += section;
        text += "\n";
        pos = text.size() - 1;
    }

    std::size_t insertPos = text.find('\n', pos);
    if (insertPos == std::string::npos) insertPos = text.size();
    ++insertPos;

    std::size_t nextSec = text.find("\n[", insertPos);
    if (nextSec == std::string::npos) nextSec = text.size();

    std::string before = text.substr(0, nextSec);
    std::string after  = text.substr(nextSec);

    if (before.size() && before.back()!='\n') before += '\n';
    before += line;
    before += '\n';

    return writeAll(path_, before + after);
}

bool CalendarHealth::addActivity(const std::string& date, const std::string& type, double durationMin, double weightKg)
{
    std::ostringstream ln;
    ln.setf(std::ios::fixed); ln.precision(2);
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
    ln.setf(std::ios::fixed); ln.precision(2);
    ln << date << ' ' << dish
       << " grams=" << grams
       << " kcal="  << kcal
       << " P="     << protein
       << " F="     << fat
       << " C="     << carbs
       << " waterL="<< waterLiters;
    return insertUnderSection("[food]", ln.str());
}

bool CalendarHealth::addCalories(const std::string& date, const std::string& dish, int calories)
{
    return addFood(date, dish, 0.0, (double)calories, 0.0, 0.0, 0.0, 0.0);
}

bool CalendarHealth::addMood(const std::string& date, int mood, const std::string& note)
{
    std::ostringstream ln;
    ln << date << ' ' << mood << ' ' << note;
    return insertUnderSection("[mood]", ln.str());
}

bool CalendarHealth::computeCaloriesForDate(const std::string& date,
                                            double& kcalIn, double& kcalOut) const
{
    kcalIn = 0.0;
    kcalOut = 0.0;

    std::ifstream in(path_);
    if (!in) return false;

    std::string line, curSec;
    while (std::getline(in, line))
    {
        std::string s = trim(line);
        if (s.empty()) continue;
        if (s.front()=='[' && s.back()==']') { curSec = s; continue; }
        if (s.size() < 10) continue;

        if (!startsWith(s, date)) continue;

        std::string payload = (s.size() > 11 ? s.substr(11) : std::string());

        if (curSec == "[food]") {
            auto pos = payload.find("kcal=");
            if (pos != std::string::npos) {
                pos += 5;
                std::size_t end = pos;
                while (end < payload.size()
                       && (std::isdigit((unsigned char)payload[end]) || payload[end]=='.'))
                    ++end;
                try {
                    double kcal = std::stod(payload.substr(pos, end-pos));
                    kcalIn += kcal;
                } catch (...) {  }
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
    double inK = 0.0, outK = 0.0;
    if (!computeCaloriesForDate(date, inK, outK)) return false;

    std::ostringstream ln;
    ln.setf(std::ios::fixed);
    ln << std::setprecision(2);
    ln << date
       << " in="  << inK
       << " out=" << outK;

    return insertUnderSection("[calories]", ln.str());
}

bool CalendarHealth::listByDate(const std::string& date, std::vector<std::string>& out) const
{
    out.clear();
    std::ifstream in(path_);
    if (!in) return false;

    std::string line, curSec;
    while (std::getline(in, line))
    {
        std::string s = trim(line);
        if (s.empty()) continue;
        if (s.front()=='[' && s.back()==']') { curSec = s; continue; }
        if (s.size() < 10) continue; // дата мінімум 10 символів

        if (startsWith(s, date))
        {
            if (curSec == "[activity]")       out.push_back("[activity] " + s.substr(11));
            else if (curSec == "[food]")      out.push_back("[food]     " + s.substr(11));
            else if (curSec == "[calories]")  out.push_back("[calories] " + s.substr(11));
            else if (curSec == "[mood]")      out.push_back("[mood]     " + s.substr(11));
            else                               out.push_back(s);
        }
    }
    return true;
}
