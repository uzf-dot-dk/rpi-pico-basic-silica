#include "pico/stdlib.h"

#include <stdio.h>
#include <silica/Application.h>
#include <silica/CoarseTimer.h>
#ifndef LED_DELAY_MS
#define LED_DELAY_MS 225
#endif

#ifndef PICO_DEFAULT_LED_PIN
#warning blink_simple example requires a board with a regular LED
#endif


#include <silica/LoggingSystem.h>
#include <stdlib.h>

#define TEXT_NORMAL  "\x1B[0m"
#define TEXT_RED     "\x1B[31m"
#define TEXT_GREEN   "\x1B[32m"
#define TEXT_YELLOW  "\x1B[33m"
#define TEXT_BLUE    "\x1B[34m"
#define TEXT_MAGENTA "\x1B[35m"
#define TEXT_CYAN    "\x1B[36m"
#define TEXT_WHITE   "\x1B[37m"

#ifdef SILICA_LOGENTRY_FILENAME_MAX_LENGTH
#undef SILICA_LOGENTRY_FILENAME_MAX_LENGTH
#endif
#define SILICA_LOGENTRY_FILENAME_MAX_LENGTH 10
class StdoutLogSink : public Silica::LogSink
{
public:
    void sinkEntry(const Silica::LogEntry &entry) override
    {
        const char *color;
        char indicator;
        switch(entry.type())
        {
        case Silica::LogEntry::Type::Log:
            color = TEXT_NORMAL;
            indicator = 'L';
            break;
        case Silica::LogEntry::Type::Warning:
            color = TEXT_YELLOW;
            indicator = 'W';
            break;
        case Silica::LogEntry::Type::Fatal:
            color = TEXT_RED;
            indicator = 'F';
            break;
        }

        printf("%s# %c : %4zu : %-*s : %s\n" TEXT_NORMAL, color, indicator, entry.originatingLine(), SILICA_LOGENTRY_FILENAME_MAX_LENGTH, entry.originatingFile(), entry.message());

        if(entry.type() == Silica::LogEntry::Type::Fatal)
        {
            Silica::Array<char, 8> wheel = {'/', '-', '\\', '|', '/', '-', '\\', '|'};
            size_t counter = 0;
            while(1) {
                counter++;
                if(counter >= wheel.size())
                {
                    counter = 0;
                }
                printf("%s# FATAL %c %4zu : %-*s : %s\r", TEXT_NORMAL,  wheel[counter], entry.originatingLine(), SILICA_LOGENTRY_FILENAME_MAX_LENGTH, entry.originatingFile(), entry.message());
                sleep_ms(500);
            }
        }

    };
};

StdoutLogSink sink;

void registerDefaultLogSink(Silica::LoggingSystem *loggingSystem)
{
    loggingSystem->setSink(&sink);
}


void Silica::Application::platformSpecificInitialization()
{
    stdio_init_all();
}

Silica::MicroSeconds Silica::Application::microsecondsSinceStart() const
{
    return MicroSeconds(time_us_64());
}



int main()
{
    Silica::Application app;
    Silica::CoarseTimer timer;
    size_t counter = 0;

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    timer.setTimeout(Silica::MilliSeconds(1000));
    timer.triggered.connectTo([&](){
        LOG("Yo, %d! I'm SUPA-LAMBDA! %d milli seconds", int(counter++), LED_DELAY_MS);
        gpio_put(PICO_DEFAULT_LED_PIN, ( (counter % 2) == 0));
        if(counter > 3)
        {
            FATAL("Counter too high!");
        }
    });
    timer.start();
    return app.exec();
}
