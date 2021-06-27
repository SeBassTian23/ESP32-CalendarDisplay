/**
 * Buzzer sounds for Calendar
 * 
 */

#include "buzzer.h"
#include "Arduino.h"

extern int GPIO_12;

void playTone(int tone, int duration)
{

    for (long i = 0; i < duration * 1000L; i += tone * 2)
    {

        digitalWrite(GPIO_12, HIGH);
        delayMicroseconds(tone);
        digitalWrite(GPIO_12, LOW);
        delayMicroseconds(tone);
    }
}

void playNote(char note, int duration)
{

    char names[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B',
                    'c', 'd', 'e', 'f', 'g', 'a', 'b',
                    'x', 'y'};

    int tones[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014,
                   956, 834, 765, 593, 468, 346, 224,
                   655, 715};

    int SPEE = 5;
    // play the tone corresponding to the note name
    for (byte i = 0; i < 16; i++)
    {

        if (names[i] == note)
        {
            int newduration = duration / SPEE;
            playTone(tones[i], newduration);
        }
    }
}

void playNotification()
{
    int length = 3;
    char notes[] = "c c";
    int beats[] = {8, 2, 8};
    int tempo = 120;

    for (byte i = 0; i < length; i++)
    {
        if (notes[i] == ' ')
        {
            delay(beats[i] * tempo); // rest
        }
        else
        {
            playNote(notes[i], beats[i] * tempo);
        }
        // pause between notes
        // delay(tempo);
    }
}

void playHappyBirthday()
{
    int length = 6;
    char notes[] = "GGAGcB GGAGdc GGxecBA yyecdc";
    int beats[] = {2, 2, 8, 8, 8, 16, 1, 2, 2, 8, 8, 8, 16, 1, 2, 2, 8, 8, 8, 8, 16, 1, 2, 2, 8, 8, 8, 16};
    int tempo = 150;

    for (byte i = 0; i < length; i++)
    {
        if (notes[i] == ' ')
        {
            delay(beats[i] * tempo); // rest
        }
        else
        {
            playNote(notes[i], beats[i] * tempo);
        }
        // pause between notes
        delay(tempo);
    }
}


