#include "IotJr.h"


// ============================================================
// Configuração padrão
// ============================================================

const String nameFirstSSID        = "ConfigureDeviceJR";
const String passwordDefault      = "12345678";
const String typeNetWorkDefault = "accesspoint";
const String nameDefaultConf     = "conf.txt";
const String nameDNS                = "ifms.local";

bool isConfiguredYet = false;


// ============================================================
// HOME
// ============================================================

void home(ServerWebJr& server)
{
    File file = SPIFFS.open("/home.html", "r");

    if (!file) {

        server.getServer().send(
            404,
            "text/plain",
            "home.html nao encontrado"
        );

        return;
    }

    String html = file.readString();

    file.close();

    server.getServer().send(
        200,
        "text/html",
        html
    );
}

// ============================================================
// CHAPA
// ============================================================

void chapa(ServerWebJr& server)
{
    File file = SPIFFS.open("/chapa.png", "r");

    if (!file) {

        server.getServer().send(
            404,
            "text/plain",
            "chapa.png nao encontrado"
        );

        return;
    }

    server.getServer().streamFile(
        file,
        "image/png"
    );

    file.close();
}


// ============================================================
// CONFIGURE
// ============================================================

void configure(ServerWebJr& server)
{
    FilesJr files;

    if (
        !server.getServer().hasArg("mode") ||
        !server.getServer().hasArg("ssid") ||
        !server.getServer().hasArg("password")
    ) {

        server.getServer().send(
            400,
            "text/plain",
            "Parametros incompletos"
        );

        return;
    }


    String typeNetWork =
        server.getServer().arg("mode");

    String SSID =
        server.getServer().arg("ssid");

    String password =
        server.getServer().arg("password");


    String line =
        typeNetWork +
        "," +
        SSID +
        "," +
        password;


    files.createFile(
        nameDefaultConf.c_str(),
        line.c_str()
    );


    isConfiguredYet = true;


    server.getServer().send(
        200,
        "text/plain",
        "Configuracao ok. Desligue e ligue "
        "[sem o reset] o dispositivo para o funcionamento!"
    );
}


// ============================================================
// CONSTRUCTOR
// ============================================================

IotJr::IotJr(
    const String& conf_file,
    int buttonReset,
    int ledInformation
)
    : conf(conf_file),
      buttonReset(buttonReset),
      ledInformation(ledInformation)
{
    msg =
        "\n"
        "-----------------------------------------------------------------------------\n";
}


// ============================================================
// INITIATED
// ============================================================

void IotJr::initiated()
{
    checkPinAnd_Reset();


    if (mountingSystemFiles()) {

        ledStartingConnectWifi();


        if (isFirstSetup()) {

            msg +=
                "-> This is the first connection and "
                "let's create page config!\n";


            wifi = WiFiJr(nameFirstSSID.c_str(), passwordDefault.c_str());
            //wifi.startDNS(nameDNS.c_str());

            if (wifi.createAccessPoint()) {

                msg +=
                    "-> The IP is...: " +
                    wifi.getAccessPointIP() +
                    "\n";


                createConfigFactory();
            }
        }

    } else {

        msg +=
            "-> Not is possible mounting the system file!\n";
    }
}


// ============================================================
// RUNNING
// ============================================================

void IotJr::running(
    Route routes[],
    int quantidade
)
{
    if (!mountingSystemFiles()) {
        return;
    }


    String line =
        files.readFile(
            conf.c_str()
        );


    extractingData(line);


    wifi = WiFiJr( SSID.c_str(), password.c_str());
    //wifi.startDNS(nameDNS.c_str());

    isConnected =
        wifi.isConnected();


    msg +=
        "-> The system is running in mode..: [" +
        typeNetWork +
        "] using the network ..:[ " +
        SSID +
        "]";


    // ========================================================
    // ACCESS POINT
    // ========================================================

    if (typeNetWork == "accesspoint") {

        if (wifi.createAccessPoint()) {

            msg +=
                " with IP [" +
                wifi.getAccessPointIP() +
                "]\n";


            server =
                new ServerWebJr(80);


            for (int i = 0; i < quantidade; i++) {

                server->addRoute(
                    routes[i].path,
                    routes[i].function
                );
            }


            server->start(
                "Servidor iniciado"
            );


            isConnected = true;

        } else {

            isConnected = false;
        }
    }


    // ========================================================
    // STATION
    // ========================================================

    else {

        if (wifi.connect()) {

            msg +=
                " with IP [" +
                wifi.getIPAddress() +
                "]\n";


            server =
                new ServerWebJr(
                    80,
                    SSID.c_str(),
                    password.c_str()
                );


            for (int i = 0; i < quantidade; i++) {

                server->addRoute(
                    routes[i].path,
                    routes[i].function
                );
            }


            server->start(
                "Servidor iniciado"
            );


            isConnected = true;

        } else {

            isConnected = false;
        }
    }
}


// ============================================================
// ERROR CONNECT WIFI
// ============================================================

void IotJr::errorConnectWifi()
{
    if (!isConnected) {

        digitalWrite(
            ledInformation,
            LOW
        );

    } else {

        digitalWrite(
            ledInformation,
            HIGH
        );
    }
}


// ============================================================
// LED STARTING
// ============================================================

void IotJr::ledStartingConnectWifi()
{
    digitalWrite(
        ledInformation,
        LOW
    );


    for (int i = 0; i < 5; i++) {

        digitalWrite(
            ledInformation,
            LOW
        );

        delay(1000);


        digitalWrite(
            ledInformation,
            HIGH
        );

        delay(1000);
    }
}


// ============================================================
// CREDENTIALS
// ============================================================

void IotJr::setCredentialsNet(
    String typeNetWork,
    String SSID,
    String password
)
{
    this->typeNetWork = typeNetWork;
    this->SSID = SSID;
    this->password = password;
}


// ============================================================
// EXTRACT DATA
// ============================================================

void IotJr::extractingData(
    String dados
)
{
    int primeiraVirgula =
        dados.indexOf(',');


    int segundaVirgula =
        dados.indexOf(
            ',',
            primeiraVirgula + 1
        );


    this->typeNetWork =
        dados.substring(
            0,
            primeiraVirgula
        );


    this->SSID =
        dados.substring(
            primeiraVirgula + 1,
            segundaVirgula
        );


    this->password =
        dados.substring(
            segundaVirgula + 1
        );
}


// ============================================================
// FILESYSTEM
// ============================================================

bool IotJr::mountingSystemFiles()
{
    return files.isFileSystemMounted();
}


// ============================================================
// FACTORY CONFIGURATION
// ============================================================

void IotJr::createConfigFactory()
{
    ServerWebJr serverFactory(80);

    serverFactory.addRoute(
        "/",
        home
    );


    serverFactory.addRoute(
        "/chapa.png",
        chapa
    );


    serverFactory.addRoute(
        "/configure",
        configure
    );


    serverFactory.start(
        "Servidor iniciado"
    );


    while (!isConfiguredYet) {
        //w.dnsServer.processNextRequest();
        serverFactory.getServer().handleClient();

        delay(1);
    }
}


// ============================================================
// FIRST SETUP
// ============================================================

bool IotJr::isFirstSetup()
{
    return !files.fileExists(
        conf.c_str()
    );
}


// ============================================================
// RESET
// ============================================================

void IotJr::checkPinAnd_Reset()
{
    int value =
        digitalRead(buttonReset);


    if (value == LOW) {

        msg +=
            "-> The button RESET is on and "
            "now we are delete config\n";


        Serial.print(msg);


        files.deleteFile(
            conf.c_str()
        );
    }
}