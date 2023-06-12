// Author: PeterMontez
// This code was made for an esp32 devboard.
// It controls a single ultrassonic sensor and an I2C 16x2 LCD display;
// The display show sentences written by me, and hosted as a simple HTML file on GitHub pages;

#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 19;
const int echoPin = 18;

long duration;
int distance;

bool now = false;
bool lastState = false;

char *strings[50];
char *strings2[3];
char *ptr = NULL;

String linha1 = "";
String linha2 = "";

const char *ssid = "";
const char *password = "";

// URL DO SITE COM AS FRASES
String serverName = "";

unsigned long lastTime = 0;

unsigned long LCDTime = 0;
unsigned long timerLCD = 10000;

unsigned long timerDelay = 5000;

void setup()
{

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    lcd.init();
    lcd.backlight();

    lcd.print("De: Peter");
    lcd.setCursor(0, 1);
    lcd.print("Para: Vivi");
    delay(5000);

    randomSeed(analogRead(0));

    Serial.begin(115200);

    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    lcd.clear();
    lcd.print("Conectando...");
     while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop()
{

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    // Serial.println(distance);
    delay(100);

    if (distance < 100 && distance >= 10)
    {
        lcd.backlight();
        lastState = false;
        LCDTime = millis();
    }
    else if ((millis() - LCDTime) > timerLCD)
    {
        lcd.noBacklight();
    }

    if (distance < 10 && !lastState)
    {
        now = true;
        lastState = true;
    }

    // Manda um POST conforme o tempo configurado
    if ((millis() - lastTime) > timerDelay || now)
    {
        now = false;

        if (WiFi.status() == WL_CONNECTED)
        {
            HTTPClient http;

            String serverPath = serverName;

            // Inicia o request usando a URL definida lá em cima
            http.begin(serverPath.c_str());

            // Manda o request
            int httpResponseCode = http.GET();

            if (httpResponseCode > 0)
            {
                // Serial.println(httpResponseCode);
                String payload = http.getString();
                // Serial.println(payload);

                char array[payload.length() + 1];
                payload.toCharArray(array, payload.length() + 1);

                byte index = 0;
                ptr = strtok(array, ";"); // Delimita o ";" de cada frase
                while (ptr != NULL)
                {
                    strings[index] = ptr;
                    index++;
                    ptr = strtok(NULL, ";");
                }

                String chosen = strings[random(0, 49)];
                char array2[chosen.length() + 1];
                chosen.toCharArray(array2, chosen.length() + 1);
                index = 0;
                ptr = strtok(array2, ","); // Delimita o "," de cada frase
                while (ptr != NULL)
                {
                    strings2[index] = ptr;
                    index++;
                    ptr = strtok(NULL, ",");
                }

                // Salva os valores separados nas linhas respectivas do LCD
                linha1 = strings2[0];
                linha2 = strings2[1];

                lcd.clear();
                lcd.print(linha1);
                lcd.setCursor(0, 1);
                lcd.print(linha2);

                // Serial.println(linha1);
                // Serial.println(linha2);
            }
            else
            {
                Serial.print("Error code: ");
                Serial.println(httpResponseCode);
            }
            // Finaliza a comunicação para liberar recursos e gastar menos bateria
            http.end();
        }
        else
        {
            Serial.println("WiFi Disconnected");
        }
        lastTime = millis();
    }
}
