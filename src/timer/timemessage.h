#ifndef TIMEMESSAGE_H
#define TIMEMESSAGE_H

class TimeMessage
{
public:
    TimeMessage(uint time_from, uint time_to)
        : from(time_from), to(time_to) {};

    uint from;
    uint to;
};

#endif // TIMEMESSAGE_H
