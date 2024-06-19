#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#define motorD0 16         // motor direita frente
#define motorD1 5         // motor direita reverso           
#define motorD2 4          //motor esquerda frente
#define motorD3 0        // motor esquerda reverso           
#define ledD4  2         // LED
#define ledD5  14         // LED
#define ledD7  13          // LED FRENTE
#define ledD8  15           //LED FRENTE
#define buzzerD6 12           //BUZINA



#define HOST "localhost"
#define WIFI_SSID "Galaxy A53 5G"
#define WIFI_PASSWORD "click7294"

String command, sendval, postData;

const char* ssid = "Galaxy A53 5G";
const char* password = "click7294";

ESP8266WebServer server(80);


const int led = 2;

void handleRoot() {
  String site = R"rawliteral(
    <!DOCTYPE html>
    <html lang="pt-br">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Controle do Robô com ESP8266</title>
        <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
        <style>
            body {
                background: url('Desing%20sem%20nome(1).png') no-repeat center center fixed;
                background-size: cover;
                color: black;
                font-family: Arial, sans-serif;
                margin: 0;
                overflow-x: hidden;
                text-align: center;
            }
            header {
                background-color: rgba(255, 4, 4, 0.7);
                color: white;
                font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;
                padding: 20px;
                text-align: center;
                position: relative;
                z-index: 2;
            }
            footer {
                background-color: rgba(0, 0, 0, 0.7);
                color: white;
                font-family: Arial, sans-serif;
                padding: 10px;
                text-align: center;
                position: fixed;
                width: 100%;
                bottom: 0;
            }
            .control-panel {
                background: rgba(255, 255, 255, 0.8);
                border-radius: 20px;
                box-shadow: 0 0 15px rgba(0, 0, 0, 0.3);
                padding: 20px;
                text-align: center;
                width: calc(100% - 40px); /* full width minus padding */
                height: calc(100vh - 160px); /* full height minus header, footer and padding */
                margin: 20px auto;
                display: flex;
                flex-direction: column;
                justify-content: center;
                align-items: center;
            }
            .btn-custom {
                background-color: rgba(255, 255, 255, 0.908);
                color: black;
                border: 2px solid black;
                width: 120px;
                height: 120px;
                font-size: 24px;
                margin: 10px;
                cursor: pointer;
                border-radius: 50%;
                transition: background-color 0.3s, transform 0.3s;
                display: flex;
                align-items: center;
                justify-content: center;
            }
            .btn-custom:hover {
                background-color: rgba(255, 255, 255, 0.9);
                transform: scale(1.1);
            }
            .btn-custom-rect {
                background-color: rgba(255, 255, 255, 0.908);
                color: black;
                border: 2px solid black;
                width: 160px;
                height: 120px;
                font-size: 24px;
                margin: 10px;
                cursor: pointer;
                border-radius: 15px;
                transition: background-color 0.3s, transform 0.3s;
                display: flex;
                align-items: center;
                justify-content: center;
            }
            .btn-custom-rect:hover {
                background-color: rgba(255, 255, 255, 0.9);
                transform: scale(1.1);
            }
            .btn-container {
                display: grid;
                grid-template-rows: 1fr 1fr 1fr;
                grid-template-columns: 1fr 1fr 1fr;
                grid-template-areas:
                    ". btn-up ."
                    "btn-left icon btn-right"
                    ". btn-down .";
                gap: 10px;
                justify-items: center;
                align-items: center;
                flex-grow: 1;
            }
            .btn-up {
                grid-area: btn-up;
            }
            .btn-left {
                grid-area: btn-left;
            }
            .btn-right {
                grid-area: btn-right;
            }
            .btn-down {
                grid-area: btn-down;
            }
            .icon {
                grid-area: icon;
                font-size: 48px;
                color: black;
            }
            .btn-bottom-container {
                display: flex;
                justify-content: center;
                gap: 10px;
                margin-top: 10px;
            }
        </style>
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css">
    </head>
    <body>
        <header>
            <h1>Controle do Carro com ESP8266</h1>
        </header>
        <div class="control-panel">
            <div class="btn-container">
                <div class="btn-up">
                    <button class="btn btn-primary btn-custom" onclick="sendCommand('F')"><i class="fas fa-arrow-up"></i></button>
                </div>
                <div class="btn-left">
                    <button class="btn btn-primary btn-custom" onclick="sendCommand('L')"><i class="fas fa-arrow-left"></i></button>
                </div>
                <div class="icon"><i class="fas fa-car"></i></div>
                <div class="btn-right">
                    <button class="btn btn-primary btn-custom" onclick="sendCommand('R')"><i class="fas fa-arrow-right"></i></button>
                </div>
                <div class="btn-down">
                    <button class="btn btn-primary btn-custom" onclick="sendCommand('B')"><i class="fas fa-arrow-down"></i></button>
                </div>
            </div>
            <div class="btn-bottom-container">
                <button class="btn btn-danger btn-custom-rect" onclick="sendCommand('S')">Parar</button>
                <button class="btn btn-warning btn-custom-rect" onclick="sendCommand('V')">Buzina</button>
            </div>
        </div>
        <footer>
            <p>&copy; 2024 Controle do Carro com ESP8266. Todos os direitos reservados.</p>
        </footer>

        <script>
            function sendCommand(cmd) {
                fetch('/comando?State=' + cmd, {
                    method: 'GET'
                })
                .then(response => response.text())
                .then(data => {
                    console.log(data);
                })
                .catch(error => {
                    console.error('Erro:', error);
                });
            }
        </script>
        <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.2/dist/umd/popper.min.js"></script>
        <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", site);
  site = "";

  digitalWrite(led, 0);
}
void handleGraph() {
  String site = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
  <canvas id="myChart"></canvas>
  <script>
    let sensorValues = [10, 20, 15, 25, 30]; // Exemplo de dados iniciais

    function addValue(value) {
      sensorValues.push(value);
    }

    function createChart() {
      const ctx = document.getElementById('myChart').getContext('2d');
      const chart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: sensorValues.map((value, index) => `Leitura ${index + 1}`),
          datasets: [{
            label: 'Valor do sensor',
            data: sensorValues,
            backgroundColor: 'rgba(255, 99, 132, 0.2)',
            borderColor: 'rgba(255, 99, 132, 1)',
            borderWidth: 1
          }]
        }
      });
    }

    function updateChart() {
      createChart();
    }

    // Get data from Arduino using WebSocket or AJAX
    // For example, using WebSocket:
    const socket = new WebSocket('ws://192.168.5.46:81');

    socket.onmessage = (event) => {
      const smokeValue = parseInt(event.data);
      addValue(smokeValue);
      updateChart();
    };

    // Initial chart creation
    createChart();
  </script>
</body>
</html>
      )rawliteral";

  server.send(200, "text/html", site);
  digitalWrite(led, 0);
}

void handleNotFound(){          //  não econtrou a URL
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void checkWiFiConnection() 
{

  while (WiFi.status() != WL_CONNECTED)
   {
    Serial.println("Connecting to Wi-Fi...");
    delay(1000);
  }

  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

}

void setup() 
{
  pinMode(motorD0, OUTPUT);
  pinMode(motorD1, OUTPUT);
  pinMode(motorD2, OUTPUT);
  pinMode(motorD3, OUTPUT);
  pinMode(ledD4, OUTPUT);
  pinMode(ledD5, OUTPUT);
  pinMode(ledD7, OUTPUT);
  pinMode(ledD8, OUTPUT);
  pinMode(buzzerD6, OUTPUT);

  WiFi.mode(WIFI_AP); //aceita WIFI_AP / WIFI_AP_STA / WIFI_STA
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  server.on("/", handleRoot);
  server.on("/sensor",handleGraph);

  server.begin();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //conecta no WI-FI
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  Serial.begin(115200);
  Serial.println("Communication Started\n");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  checkWiFiConnection();
}
void BeepHorn() {
    digitalWrite(buzzerD6, HIGH); // Ligar o buzzer
  }

void goAhead() {
  digitalWrite(motorD0, HIGH);
  digitalWrite(motorD1, LOW);
  digitalWrite(motorD2, HIGH);
  digitalWrite(motorD3, LOW);
}

void goBack() {
// Acende o LED no pino 
  digitalWrite(ledD4, HIGH);
  digitalWrite(ledD5, HIGH);
  delay(500); // Espera 500 milissegundos
  
  // Apaga o LED no pino
  digitalWrite(ledD4, LOW);
  digitalWrite(ledD5, LOW);
  delay(300); // Espera 500 milissegundos
// Buzzer ativa 
  digitalWrite(buzzerD6, HIGH);
  delay(100);
  digitalWrite(buzzerD6, LOW);


  digitalWrite(motorD0, LOW);
  digitalWrite(motorD1, HIGH);
  digitalWrite(motorD2, LOW);
  digitalWrite(motorD3, HIGH);
}

void goRight() {
 digitalWrite(motorD0, LOW);
  digitalWrite(motorD1, HIGH);
  digitalWrite(motorD2, HIGH);
  digitalWrite(motorD3, LOW);
}

void goLeft() {
  digitalWrite(motorD0, HIGH);
  digitalWrite(motorD1, LOW);
  digitalWrite(motorD2, LOW);
  digitalWrite(motorD3, HIGH);
}

void stopRobot() {
  digitalWrite(motorD0, LOW);
  digitalWrite(motorD1, LOW);
  digitalWrite(motorD2, LOW);
  digitalWrite(motorD3, LOW);
  digitalWrite(buzzerD6, LOW);
  }

void loop()
 {
  
  // Acende o LED FRONTAL
  digitalWrite(ledD8, HIGH);
  digitalWrite(ledD7, HIGH);

  
  // Acende o LED TRASEIRA
  digitalWrite(ledD4, HIGH);
  digitalWrite(ledD5, HIGH);

  server.handleClient();
  
  command = server.arg("State");
  if (command == "F") goAhead();
  else if (command == "B") goBack();
  else if (command == "L") goLeft();
  else if (command == "R") goRight();
  else if (command == "V") BeepHorn();
  else if (command == "S") stopRobot();
  
  checkWiFiConnection();
  WiFiClient client;
  HTTPClient http;

  int smoke = analogRead(A0); // Lê o valor analógico da porta A0
  sendval = String(smoke);
  postData = "sendval=" + sendval;
  if (smoke > 300) {
    // Dispara o buzzer
    digitalWrite(buzzerD6, HIGH);
    delay(1000);
    digitalWrite(buzzerD6, LOW);
  } else {


  }

  Serial.print("Sending data: ");
  Serial.println(postData);

  http.begin(client, "http://192.168.98.125/teste_sensor/sensor.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);

  if (httpCode == HTTP_CODE_OK) 
  {
    String response = http.getString();
    Serial.println("Server response: " + response);

  } 
  
  else
  {
    Serial.print("HTTP POST request failed with error code: ");
    Serial.println(httpCode);
    if (httpCode == HTTPC_ERROR_CONNECTION_REFUSED)
    {
      Serial.println("Connection refused by the server.");
    }
    else if (httpCode == HTTP_CODE_NOT_FOUND) 
    {
      Serial.println("Server resource not found.");
    }
    else {
      Serial.println("Unknown error occurred.");
    }
  }

 http.end();
      delay(1000);
 
}