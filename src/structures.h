#ifndef EVENT_H_
#define EVENT_H_

  struct Event_type {
    time_t        starttime;
    time_t        endtime;
    const char*   description;
    const char*   location;
    const char*   categories;
    bool          alarm;
    bool          allday;
  };

#endif

#ifndef WEATHER_H_
#define WEATHER_H_

  struct Weather_type {
    const char*   summary;
    const char*   icon;
    float         temperature;
  };

#endif