#include "IotJr2.h"

//---------------- All functions for calling in server ----------------------------
// ============================================================
// HOME
// ============================================================

void home(ServerWebJr &server)
{
    File file = SPIFFS.open("/home.html", "r");
    if (!file)
    {
        server.getServer().send(404, "text/plain", "home.html nao encontrado");
        return;
    }
    String html = file.readString();
    file.close();
    server.getServer().send(200, "text/html", html);
}

// ============================================================
// CHAPA
// ============================================================

void chapa(ServerWebJr &server)
{
    File file = SPIFFS.open("/chapa.png", "r");
    if (!file)
    {

        server.getServer().send(404, "text/plain", "chapa.png nao encontrado");
        return;
    }
    server.getServer().streamFile(file, "image/png");
    file.close();
}

// ============================================================
// CONFIGURE
// ============================================================

void configure(ServerWebJr &server)
{
    FileOperation file;
    if (
        !server.getServer().hasArg("mode") ||
        !server.getServer().hasArg("ssid") ||
        !server.getServer().hasArg("password"))
    {

        server.getServer().send(400, "text/plain", "Parametros incompletos");
        return;
    }
    String typeNetWork = server.getServer().arg("mode");
    String ssid = server.getServer().arg("ssid");
    String password = server.getServer().arg("password");
    networktype type;
    String nameType;

    if (typeNetWork == "accesspoint")
    {
        type = POINTACCESS;
        nameType = "Access Point";
    }
    else
    {
        type = NETWORK;
        nameType = "Network";
    }

    file.setData(ssid, password, NAME_DNS, true, type);
    file.fileWrite();

    //---------------------------------- mensage  -------------------------------------
    if (DEBUG_MSG)
    {
        Serial.println("++++++++++++++++++++++++++ Configure +++++++++++++++++++++++++++++++++");
        Serial.println("+++ SSID ...:[" + ssid + "]");
        Serial.println("+++ DNS ....:[" + String(NAME_DNS) + "]");
        Serial.println("+++ Type Net...:[" + nameType + "]");
        Serial.println("++++++++++++++++++++++++++ Configure +++++++++++++++++++++++++++++++++");
    }
    //---------------------------------------------------------------------------------

    server.getServer().send(200, "text/plain", "Configuracao ok. Desligue e ligue [sem o reset] o dispositivo para o funcionamento!");
}

//---------------------------------------------------------------------------------


ServerIoT::ServerIoT(int buttonReset, int ledInformation)
{
    this->buttonReset = buttonReset;
    this->ledInformation = ledInformation;
}

void ServerIoT::initiated(Route routes[], int quantidade)
{
    // Check if pin reset is on and remove the file configuration
    this->checkPinAndReset();

    // Check if file exists!
    bool exists = file.fileExists();
    if (!exists)
    {
        file.fileWrite();
        if (DEBUG_MSG)
        {
            Serial.println("|  - Let's create new file configuration with values factory!");
        }
    }
    file.fileRead();
    data dat = file.getData();
    wifi = WiFiJr(dat.ssid, dat.password);

    if (dat.typeNet == CONFIGURE && dat.isConfigured == false)
    {
        firstTime = true;
        if (DEBUG_MSG)
        {
            Serial.println("Factory setting is [ON]");
            Serial.println("The SSID is [" + String(dat.ssid) + "] ..........");
        }
        if (wifi.createAccessPoint())
        {
            wifi.startDNS(dat.dns);
            if (DEBUG_MSG)
            {
                Serial.println("|  - The ip access is ...:[" + wifi.getAccessPointIP() + "] ..................");
            }
            createConfigFactory();
        }
    }
    else
    {

        if (dat.typeNet == POINTACCESS)
        {
            if (wifi.createAccessPoint())
            {
                wifi.startDNS(NAME_DNS);
                if (DEBUG_MSG)
                {
                    Serial.println("|  - The ssid is [" + String(dat.ssid) + "]");
                    Serial.println("|  - The connection with IP [" + wifi.getAccessPointIP() + "]\n");
                }
                server = new ServerWebJr(80);
                for (int i = 0; i < quantidade; i++)
                {

                    server->addRoute(
                        routes[i].path,
                        routes[i].function);
                }
                server->start("Servidor iniciado");
            }
            else
            {
                if (DEBUG_MSG)
                {
                    Serial.println("|  -Problem with connection: ");
                    Serial.println("|  - The ssid is [" + String(dat.ssid) + "]");
                    Serial.println("|  - The connection with IP [" + wifi.getAccessPointIP() + "]\n");
                }
            }
        }
        else
        {
            if (wifi.connect())
            {
                wifi.startMDNS(NAME_DNS);
                if (DEBUG_MSG)
                {
                    Serial.println("|  - The ssid is [" + String(dat.ssid) + "]");
                    Serial.println("|   - Connection  with IP [" + wifi.getIPAddress() + "]\n");
                }
                server = new ServerWebJr(80, dat.ssid, dat.password);
                for (int i = 0; i < quantidade; i++)
                {

                    server->addRoute(
                        routes[i].path,
                        routes[i].function);
                }

                server->start("Servidor iniciado");
            }
        }
    }
}

void ServerIoT::checkPinAndReset()
{
    if (DEBUG_MSG)
    {
        Serial.println(" ----> Testing the button of reset is [On or Off] ");
    }

    int value = digitalRead(buttonReset);
    if (value == HIGH)
    {
        if (DEBUG_MSG)
        {
            Serial.println(" ---> The button RESET is [ON] and  now we are delete config");
        }

        this->file.fileRemove();
        if (DEBUG_MSG)
        {
            Serial.println(" ---> We are removing this file...: " + this->file.nameDefaultFile);
        }
    }
}

void ServerIoT::createConfigFactory()
{
    ServerWebJr serverFactory(80);
    serverFactory.addRoute("/", home);
    serverFactory.addRoute("/chapa.png", chapa);
    serverFactory.addRoute("/configure", configure);
    serverFactory.start("Servidor iniciado");

    file.fileRead();
    data dat = file.getData();

    while (dat.isConfigured == false)
    {
        wifi.handleDNS();
        serverFactory.getServer().handleClient();

        file.fileRead();
        dat = file.getData();
        ledStartingConnectWifi(100);
    }
}

void ServerIoT::ledStartingConnectWifi(int time_wait)
{
    digitalWrite(ledInformation, LOW);
    delay(time_wait);
    digitalWrite(ledInformation, HIGH);
    delay(time_wait);
}

void ServerIoT::loop_()
{
    bool exists = file.fileExists();
    if (exists)
    {
        file.fileRead();
        data dat = file.getData();
        if (firstTime == false)
        {
            if (dat.typeNet == POINTACCESS)
            {
                wifi.handleDNS();
            }
            else
            {
                wifi.handleMDNS();
            }
            server->getServer().handleClient();
        }
    }
}
