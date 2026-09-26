# experimentsJr

## Dependencias e requisitos

Esta biblioteca foi escrita para placas **ESP32**. Antes de usá-la, instale o suporte a ESP32 no Arduino IDE pelo Boards Manager. Esse pacote fornece as bibliotecas `WiFi`, `WebServer`, `FS` e `SPIFFS` usadas pelo projeto; não há dependências externas adicionais declaradas.

Selecione depois a placa ESP32 correta em **Ferramentas > Placa** e a porta serial correspondente. O suporte a SPIFFS depende da placa e da configuração escolhidas.

## Funcionalidades

Os módulos ficam no diretório `src/`:

- **`wifiJr.h` / `wifiJr.cpp` — conexão Wi-Fi:** `WiFiJr(ssid, senha)` guarda as credenciais; `connect()` inicia a conexão e aguarda até conectar; `isConnected()` informa o estado; `getIPAddress()` retorna o endereço IP local como `String`. Como `connect()` espera em loop, ele pode bloquear indefinidamente se a rede não estiver disponível.
- **`serverweb.h` / `serverweb.cpp` — servidor HTTP:** `ServerJr` representa um servidor `WebServer`; `addRoute(caminho, callback)` registra uma rota, `start(mensagem)` inicia o servidor e `getServer()` dá acesso ao objeto `WebServer`, por exemplo para chamar `handleClient()` e `send()`.
- **`manageFiles.h` / `manageFiles.cpp` — arquivos em SPIFFS:** `FilesJr` tenta montar o SPIFFS ao ser criado; `isFileSystemMounted()` consulta o resultado; `listFiles()` lista arquivos no Serial; `createFile(conteudo)` grava conteúdo; `readFile(conteudo)` lê o arquivo; `fileExists(caminho)` verifica existência; e `deleteFile(caminho)` remove um arquivo.

## Exemplos de uso

### Wi-Fi

```cpp
#include "wifiJr.h"

WiFiJr wifi("NOME_DA_REDE", "SENHA_DA_REDE");

void setup() {
	Serial.begin(115200);
	wifi.connect();

	if (wifi.isConnected()) {
		Serial.println(wifi.getIPAddress());
	}
}

void loop() {}
```

### Servidor web

O fluxo previsto é registrar uma rota, iniciar o servidor e atender clientes continuamente:

```cpp
#include "serverweb.h"
#include <WiFi.h>

ServerJr server(80, "NOME_DA_REDE", "SENHA_DA_REDE");

void handleRoot() {
	server.getServer().send(200, "text/plain", "Ola!");
}

void setup() {
	Serial.begin(115200);
	WiFi.begin("NOME_DA_REDE", "SENHA_DA_REDE");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}

	server.addRoute("/", handleRoot);
	server.start("Servidor iniciado");
}

void loop() {
	server.getServer().handleClient();
}
```

### SPIFFS

```cpp
#include "manageFiles.h"

FilesJr files;

void setup() {
	Serial.begin(115200);
	if (files.isFileSystemMounted()) {
		files.listFiles();
	}
}

void loop() {}
```

> **Estado atual:** os exemplos de `ServerJr` e `FilesJr` documentam o uso pretendido, mas essas implementações ainda têm incompatibilidades entre declarações e definições. Em `FilesJr`, também não há parâmetro ou configuração pública para definir o caminho usado por `createFile()` e `readFile()`. Esses dois módulos precisam desses ajustes antes de serem considerados prontos para compilação e uso. O exemplo de Wi-Fi corresponde à implementação atual.

## Instalação no Arduino IDE

1. Instale primeiro o suporte à placa ESP32 em **Ferramentas > Placa > Gerenciador de placas** (ou **Tools > Board > Boards Manager**), procurando por **esp32** e instalando o pacote **esp32 by Espressif Systems**.
2. Baixe este repositório como arquivo ZIP. No GitHub, use **Code > Download ZIP**.
3. No Arduino IDE, escolha **Sketch > Incluir Biblioteca > Adicionar biblioteca .ZIP...** (ou **Sketch > Include Library > Add .ZIP Library...**) e selecione o ZIP baixado.
4. Reinicie o Arduino IDE se a biblioteca não aparecer de imediato. Inclua no sketch o cabeçalho do módulo desejado e selecione uma placa ESP32 em **Ferramentas > Placa**.

Também é possível instalar manualmente: extraia o ZIP para a pasta `libraries` da sua pasta de sketches do Arduino, mantendo os arquivos `library.properties` e `src/` dentro da pasta da biblioteca; depois reinicie o IDE.