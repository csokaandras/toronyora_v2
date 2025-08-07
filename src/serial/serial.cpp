#include "serial.h"

String typed;
String prevTyped;

void handleSerialCommand()
{
    if (Serial.available() > 0)
    {
        char incomingByte = Serial.read();

        switch (incomingByte)
        {
        case 10:
            typed = "";
            prevTyped = "";
            Serial.println();
            break;

        case 27:
            Serial.println();
            monitorTime = false;
            monitorSensor = false;
            typed = "";
            prevTyped = "";
            break;

        case 13:
/*
            if (prevTyped == "-d")
            {
                DateTime newTime;
                newTime.year = typed.substring(0, 4).toInt();
                newTime.month = typed.substring(5, 7).toInt();
                newTime.day = typed.substring(8, 10).toInt();
                newTime.hour = currentTime.getHour();
                newTime.min = currentTime.getMinutes();
                newTime.sec = currentTime.getSeconds();

                Serial.println();
                printDate(convert2RTC(newTime), "Új dátum: ");
                RTCTime newRtc = convert2RTC(newTime);
                RTC.setTime(newRtc);
            }

            if (prevTyped == "-t")
            {
                DateTime newTime;
                newTime.year = currentTime.getYear();
                newTime.month = convert2DT(currentTime).month;
                newTime.day = currentTime.getDayOfMonth();
                newTime.hour = typed.substring(0, 2).toInt();
                newTime.min = typed.substring(3, 5).toInt();
                newTime.sec = typed.substring(6, 8).toInt();

                Serial.println();
                printDate(newTime, "Új idő: ");
                RTCTime newRtc = convert2RTC(newTime);
                RTC.setTime(newRtc);
            }

            if (prevTyped == "-r+")
            {
                if (typed.length() >= 1)
                {
                    manual_rotate_count = typed.toInt();
                }
                else
                {
                    manual_rotate_count = 1;
                }
                manual_rotate = true;
                Serial.println();
            }

            if (prevTyped == "-r-")
            {
                if (typed.length() >= 1)
                {
                    manual_rotate_count = typed.toInt() * -1;
                }
                else
                {
                    manual_rotate_count = -1;
                }
                manual_rotate = true;
                Serial.println();
            }
*/
            break;

        default:
            Serial.print(char(incomingByte));
            typed += char(incomingByte);
            break;
        }

        if (typed == "VT100")
        {
            PrintVT100();
            typed = "";
        }

        if (typed == "-ciac")
        {
            Serial.println("\n");
            Serial.println(" ╱|、");
            Serial.println("(˚ˎ。7");
            Serial.println("|、˜〵");
            Serial.println("じしˍ,)ノ");
            Serial.println();
            typed = "";
        }

        if (typed == "-h")
        {
            Serial.println("\n");
            Serial.println("VT100   kezdőlap megjelenítése");
            Serial.println("-h      segítség kiírása");
            Serial.println("-m      idő folyamatos megjelenítésének bekapcsolása, megállításához nyomja meg az ESC gombot");
            Serial.println("-t      aktuális idő beállítása");
            Serial.println("-d      aktuális dátum beállítása");
            Serial.println("-s      érzékelők állapotának megjelenítése, megállításához nyomja meg az ESC gombot");
            Serial.println("-e      error code kiirasa");
            // Serial.println("-r-     motor léptetése előre (Ha nincs érték akkor 1-el)");
            // Serial.println("-r+     motor léptetése hátra (Ha nincs érték akkor 1-el)");
            Serial.println();
            Serial.println("FONTOS");
            Serial.println("Ha törölni kell egy sorban akkor nyomjon ENTERT-t és kezdje előlről, nem tud karaktert törölni!");
            Serial.println();
            typed = "";
        }

        if (typed == "-m")
        {
            Serial.println();
            Serial.println("Megállításhoz nyomja meg az ESC gombot");
            Serial.println();
            monitorTime = true;
            typed = "";
        }

        if (typed == "-d")
        {
            Serial.println();
            Serial.println("Aktuális dátum beállítása");
            Serial.println("Elfogadáshoz nyomja meg a TAB gombot");
            Serial.println("Kérem ebben a formátumban adja meg ÉÉÉÉ/HH/NN");
            prevTyped = typed;
            typed = "";
        }

        if (typed == "-t")
        {
            Serial.println();
            Serial.println("Aktuális idő beállítása");
            Serial.println("Elfogadáshoz nyomja meg a TAB gombot");
            Serial.println("Kérem ebben a formátumban adja meg óó:pp:mm");
            prevTyped = typed;
            typed = "";
        }

        if (typed == "-s")
        {
            Serial.println();
            Serial.println("Megállításhoz nyomja meg az ESC gombot");
            Serial.println();
            monitorSensor = true;
            typed = "";
        }

        if (typed == "-e")
        {
            Serial.println();
            Serial.print("Erro code:    ");
            Serial.println(errCode);
            Serial.println();
            typed = "";
        }
        
        if (typed == "-0")
        {
            // lenullazza az error kodot
            EEPROM.put(errEeAdr, 0);
            errCode = 0;
            Serial.println();
            typed = "";
        }
/*
        if (typed == "-r+")
        {
            Serial.println();
            // Serial.println("A TAB lenyomásával lehet egyesével léptetni");
            // Serial.println();
            prevTyped = typed;
            typed = "";
        }

        if (typed == "-r-")
        {
            Serial.println();
            // Serial.println("A TAB lenyomásával lehet egyesével léptetni");
            // Serial.println();
            prevTyped = typed;
            typed = "";
        }
*/
    }
}
