# An ESP32 Calendar Display

> 3-color eInk Calendar Display

<!-- ![Calendar](./images/calndar-display.jpg) -->

## The Project

Most eInk based calendar projects are using a Raspberry Pi to display the calendar or having a server render an image that is displayed. Hoping to cut the cord in this project and just use an battery powered ESP32. A server is still required to parse the calendar files, but the ESP32 is doing the rendering and displaying, so the server only has to provide a very simple data structure.

## Hardware

+ Micro-Controller: [Adafruit HUZZAH32 – ESP32 Feather Board](https://www.adafruit.com/product/3405)
+ Display: [7.5inch HD e-Paper E-Ink Raw Display, 880×528, Black / White / Red](https://www.waveshare.com/product/displays/e-paper/epaper-1/7.5inch-hd-e-paper-hat-b.htm)
+ Display-Driver: [Universal e-Paper Raw Panel Driver HAT](https://www.waveshare.com/e-paper-driver-hat.htm)
+ Battery: [Dantona Industries - 3.7V 5200mAh W/CONN](https://www.digikey.com/en/products/detail/L37A52-2-1-2WX/3145-L37A52-2-1-2WX-ND/13692687)

### Schematic

<!-- ![Schematic](./images/schematic.png) -->

### Wiring

|   ESP32   | Display |  RTC  | Piezo Buzzer |
| :-------: | :-----: | :---: | :----------: |
|   3.3V    |   VCC   |  VCC  |     ---      |
|    GND    |   GND   |  GND  |     GND      |
|  SCK (5)  |   CLK   |  ---  |     ---      |
| MISO (19) |   ---   |  ---  |     ---      |
| MOSI (18) |   DIN   |  ---  |     ---      |
|  A7 (32)  |  BUSY   |  ---  |     ---      |
|  A8 (15)  |   CS    |  ---  |     ---      |
| A10 (27)  |   RST   |  ---  |     ---      |
|  A9 (33)  |   DC    |  ---  |     ---      |
|  A9 (33)  |   DC    |  ---  |     ---      |
|    SDA    |   ---   |  SDA  |     ---      |
|    SCL    |   ---   |  SCL  |     ---      |
| A11 (12)  |   ---   |  ---  |     3.3V     |

## Calendar Source (ical)

As it turns out, there is not a simple way to parse ical files on the ESP32. Most eInk based calendar projects are using a Raspberry Pi to display the calendar or having a server render an image that is displayed. Here a server is just providing a JSON formatted data structure with all the upcoming events. The ESP32 is used to render the calendar and events, making the source fairly flexible, so it could be a server in a home network or some other server.

### The iCal Server

In the current setup a Raspberry Pi in a local network is providing an API endpoint that is called to provide a JSON with the next up to 20 events over the next 14 days. Three calendars are providing the events, including one for holidays, one for birthdays and one for event calendar. In theory, this can be any number of calendars.

Find the implementation of the simple server [here](https://github.com/SeBassTian23/Server-CalendarDisplay), but ultimately it could be any source.

### JSON Structure

The API returns a JSON with the structure as shown below. So if you are not using the iCal format parsed as a JSON, just make sure to use the same structure and the events can basically come from any source.

```JavaScript
{
  "msg": "19 events found.",
  "count": 19,
  "gmtoffset": -240,
  "events": [
    {
      "type": "VEVENT",
      "uid": "6CD20D5D-63A9-42E8-B7DE-F8942FF6FD06",
      "start": 1619013600,
      "end": 1619017200,
      "allday": false,
      "tzid": "America/Detroit",
      "categories": null,
      "summary": "Business Meeting",
      "location": "1234 Street, Sample City, State, 12345 United States",
      "gmtoffset": -240
    },
    {
      "type": "VEVENT",
      "uid": "35508bd6-099b-3788-b81e-287d65e1e11b",
      "start": 1619064000,
      "end": 1650460531,
      "allday": true,
      "tzid": null,
      "categories": "Holidays",
      "summary": "Earth Day",
      "location": null,
      "gmtoffset": -240
    },
    ...
  ]
}
```

## Weather

In order for the current weather to be displayed, an account with [OpenWeatherMap](https://openweathermap.org/) is needed. The account can be set up for free with a limited number of API queries per day. Add the received API key to the `credentials.h`, as well as the zip and country code.
