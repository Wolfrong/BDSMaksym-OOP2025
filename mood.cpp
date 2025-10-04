#include "mood.h"
#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdlib>

    void copy_(char* d, std::size_t cap, const char* s)
    {
        if(!d||!cap){return;} if(!s){d[0]='\0';return;}
        std::size_t i=0; for(; s[i] && i+1<cap; ++i) d[i]=s[i]; d[i]='\0';
    }

    void trimNl_(char* s)
    { std::size_t n=std::strlen(s);
        while(n && (s[n-1]=='\n'||s[n-1]=='\r')) s[--n]='\0';
    }

    void currDate_(char* buf, std::size_t cap)
    {
        std::time_t t=std::time(nullptr); std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm,&t);
#else
        localtime_r(&t,&tm);
#endif
        std::strftime(buf,cap,"%Y-%m-%d",&tm);
    }

    void seedOnce_(){ static int s=0; if(!s){ std::srand((unsigned)std::time(nullptr)); s=1; } }

Mood::Mood (const char* log, const char* jokes)
{
    copy_(logPath_,  sizeof logPath_,  log   ? log   : "mood_log.txt");
    copy_(jokePath_, sizeof jokePath_, jokes ? jokes : "anekdoty.txt");
    seedOnce_();
}

int Mood::setMood(int v){ if(v<1||v>5) return 0; mood_=v; return 1; }

int Mood::save() const
{
    if(mood_==0) return 0;
    std::FILE* f = std::fopen(logPath_, "ab");
    if(!f) return 0;
    char date[16]; currDate_(date, sizeof date);
    std::fprintf(f, "%s,%d\n", date, mood_);
    std::fclose(f);
    return 1;
}

int Mood::randomJokeIfBad(char* out, std::size_t cap) const
{
    if(!out||!cap||mood_>2) return 0; out[0]='\0';
    std::FILE* f = std::fopen(jokePath_, "rb"); if(!f) return 0;
    char line[1024]; unsigned long seen=0;
    while(std::fgets(line,(int)sizeof line,f)){
        trimNl_(line); if(!line[0]) continue; ++seen;
        if((unsigned)(std::rand()%(int)seen)==0) copy_(out,cap,line);
    }
    std::fclose(f); return out[0]!='\0';
}
