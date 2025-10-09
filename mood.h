#ifndef MOOD_TRACKER_H
#define MOOD_TRACKER_H
#include <cstddef>

class Mood {
public:
    Mood(const char* pathLog = "mood_log.txt",
        const char* pathJoke = "anekdoty.txt");
    int  setMood(int v);
    int  save() const;
    int  randomJoke(char* outBuf, std::size_t outCap) const;
private:
    int  mood_;
    char logPath_[256];
    char jokePath_[256];
};
#endif
