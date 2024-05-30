#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "esp32_eoss3_spi.h"
#include "geofun.h"
#include "matfun.h"


#define GPIO_OUTPUT_VAL_REG 0x40021004
#define GPIO_OUTPUT_DIR_REG 0x40021008
#define PIN_BLUE 18
#define PIN_GREEN 21
#define PIN_RED 22
#define PIN_ALL (1<<PIN_GREEN) | (1<<PIN_BLUE) | (1<<PIN_RED)

AsyncWebServer server(80);

const char* ssid = "nani";
const char* password = "nani12345";

const char* PARAM_X1 = "x1";
const char* PARAM_Y1 = "y1";
const char* PARAM_X2 = "x2";
const char* PARAM_Y2 = "y2";
const char* PARAM_X3 = "x3";
const char* PARAM_Y3 = "y3";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Triangle Side Lengths</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Times New Roman; display: inline-block; text-align: center;}
    h2 {font-size: 2.0rem; color: orange;}
  </style> 
  <script>
    function updateResults(response) {
      document.getElementById("results").innerHTML = response;
    }

    function calculate() {
      var form = document.getElementById("triangleForm");
      var formData = new FormData(form);

      fetch('/triangle', {
        method: 'POST',
        body: formData
      })
      .then(response => response.text())
      .then(data => updateResults(data));
    }
  </script>
  </head>
  <body>
  <h2>Sides and angles of a Triangle</h2>
  <form id="triangleForm" action="/triangle" method="POST">
    <label for="x1">Vertex 1 X:</label>
    <input type="text" name="x1" value="1" required>
    <label for="y1">Y:</label>
    <input type="text" name="y1" value="-1" required><br>

    <label for="x2">Vertex 2 X:</label>
    <input type="text" name="x2" value="-4" required>
    <label for="y2">Y:</label>
    <input type="text" name="y2" value="6" required><br>

    <label for="x3">Vertex 3 X:</label>
    <input type="text" name="x3" value="-3" required>
    <label for="y3">Y:</label>
    <input type="text" name="y3" value="-5" required><br><br>

    <input type="button" value="Calculate" onclick="calculate()">
  </form><br>
  <div id="results"></div><br>
  <a href="/">Return to Home Page</a>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  esp32_eoss3_spi_init();
  uint32_t dirval = (1<<PIN_GREEN) | (1<<PIN_BLUE) | (1<<PIN_RED);
  uint32_t gpioval = 0;
  esp32_eoss3_spi_ahb_write(GPIO_OUTPUT_DIR_REG, (uint8_t *)&dirval, 4);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(2000);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/triangle", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/triangle", HTTP_POST, [&gpioval](AsyncWebServerRequest *request){
      
    double **A,**B,**C,side1, side2, side3, m=2, n=1, p=1;
    double **s_ab, **s_bc, **s_ca, angleA;
    double **a_ba,**tran_ba;

  
    int x1 = request->arg(PARAM_X1).toInt();
    int y1 = request->arg(PARAM_Y1).toInt();
    int x2 = request->arg(PARAM_X2).toInt();
    int y2 = request->arg(PARAM_Y2).toInt();
    int x3 = request->arg(PARAM_X3).toInt();
    int y3 = request->arg(PARAM_Y3).toInt();

    A = createMat(m,n);
    B = createMat(m,n);
    C = createMat(m,n);
    s_ab = createMat(m,n);
    s_bc = createMat(m,n);
    s_ca = createMat(m,n);
    
    A[0][0] = x1;
    A[1][0] = y1;
    B[0][0] = x2;
    B[1][0] = y2;
    C[0][0] = x3;
    C[1][0] = y3;
    
    s_ab = Matsub(A,B,m,n);//A-B
    s_bc = Matsub(B,C,m,n);//B-C
    s_ca = Matsub(C,A,m,n);//C-A
    a_ba = Matsub(B,A,m,n);//B-A
    
    side1 = Matnorm(s_ab,m);
    side2 = Matnorm(s_bc,m); 
    side3 = Matnorm(s_ca,m);
    tran_ba = transposeMat(a_ba,m,n);
    angleA = calculateAngleA(x1, y1, x2, y2, x3, y3);
    
    freeMat(A,2);
    freeMat(B,2);
    freeMat(C,2);
    freeMat(s_ab,2);
    freeMat(s_bc,2);
    freeMat(s_ca,2);
    freeMat(a_ba,2);



    // Create a response
    String response = "Side 1: " + String(side1) + "<br>Side 2: " + String(side2) + "<br>Side 3: " + String(side3) + "<br>angle A: " + String(angleA);

    // Send the response
    request->send(200, "text/html", response);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop(){}
