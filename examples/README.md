# Exemplos da biblioteca experimentsJr

Este diretório reúne exemplos prontos para testar as principais funcionalidades da biblioteca em placas ESP32.

Antes de usar qualquer exemplo:

1. Instale o suporte para ESP32 no Arduino IDE.
2. Selecione a placa correta em `Ferramentas > Placa`.
3. Abra o sketch desejado e ajuste os valores de rede, como SSID e senha.
4. Faça upload para a placa e acompanhe as mensagens no Monitor Serial.

## Visão geral dos exemplos

| Pasta / arquivo | O que faz | Como usar |
| --- | --- | --- |
| `iot/exampleiot.ino` | Exemplo completo de dispositivo IoT com configuração automática, rede Wi-Fi e servidor web. | Usa a classe `IotJr`, cria rota `/` e `/sensors`, e pode iniciar em modo Access Point ou Estação. |
| `manageFiles/exampleManageFile.ino` | Demonstra manipulação de arquivos em SPIFFS. | Cria, lê, verifica e remove arquivos no sistema de arquivos interno da ESP32. |
| `serverweb/exampleserverweb.ino` | Exemplo de servidor web simples com Wi-Fi conectado. | Conecta na rede Wi-Fi e expõe uma rota `/` que responde "Ola!". |
| `wifi/examplewifi.ino` | Conecta a ESP32 em uma rede Wi-Fi existente. | Ajuste o SSID e senha da rede e monitore o IP recebido. |
| `wifi/examplewifipointacess.ino` | Cria um ponto de acesso Wi-Fi. | A ESP32 gera uma rede própria para conectar dispositivos e servir páginas locais. |

---

## 1) `iot/exampleiot.ino`

### O que faz

Este é o exemplo mais completo do projeto. Ele usa a classe `IotJr` para:

- preparar a placa para funcionar como dispositivo IoT;
- configurar credenciais da rede em arquivo local;
- iniciar em modo Access Point quando necessário;
- conectar em modo Estação quando já houver configuração;
- expor rotas HTTP para servir páginas e dados JSON.

No código, há duas rotas principais:

- `/` → lê um arquivo HTML (`/read.html`) e retorna a página;
- `/sensors` → retorna um JSON com valores simulados de temperatura, umidade e pressão.

### Como usar

1. Abra `examples/iot/exampleiot.ino` no Arduino IDE.
2. Verifique que a placa está definida como ESP32.
3. Faça upload.
4. Na primeira execução, a ESP32 pode entrar em modo de configuração e criar uma rede de acesso (Access Point) com o nome padrão.
5. Conecte seu celular ou notebook nessa rede.
6. Acesse a URL informada pelo código e configure o SSID/senha da rede Wi-Fi desejada.
7. Reinicie o dispositivo para partir para o modo de operação normal.

### Observações

- O arquivo `read.html` precisa existir no sistema de arquivos SPIFFS.
- Este exemplo é ideal para projetos que precisam de uma interface local e de configuração inicial.

---

## 2) `manageFiles/exampleManageFile.ino`

### O que faz

Este exemplo demonstra como usar a classe `FilesJr` para:

- montar o sistema de arquivos SPIFFS;
- criar um arquivo com conteúdo de texto;
- ler o conteúdo do arquivo;
- verificar se ele existe;
- excluir o arquivo quando necessário.

### Como usar

1. Abra `examples/manageFiles/exampleManageFile.ino`.
2. Faça upload para a ESP32.
3. O código cria um arquivo chamado `conf.txt` com um valor de exemplo.
4. Pode alterar o conteúdo conforme a necessidade do seu projeto.

### Exemplo de operação

No sketch, a criação do arquivo é feita assim:

```cpp
if (files.createFile("conf.txt", "192.168.0.1, acesspoint")) {
    Serial.println("File created successfully!");
}
```

Você pode ativar também a leitura do arquivo e a checagem de existência comentando as linhas correspondentes.

### Observações

- Para que a gravação funcione, o SPIFFS precisa estar montado corretamente.
- Esse exemplo é útil para salvar credenciais, configurações e outros parâmetros persistentes.

---

## 3) `serverweb/exampleserverweb.ino`

### O que faz

Este exemplo conecta a ESP32 em uma rede Wi-Fi e inicializa um servidor HTTP simples.

Ele registra a rota:

- `/` → responde com a string `Ola!`

### Como usar

1. Abra `examples/serverweb/exampleserverweb.ino`.
2. Altere os valores de `SSID` e `PASSWORD` para os da sua rede.
3. Faça upload para a ESP32.
4. Acesse o IP mostrado no monitor serial em um navegador.

### Exemplo de configuração

```cpp
ServerWebJr server(80, "SSID", "PASSWORD");
WiFiJr wifi("SSID", "PASSWORD");
```

### Observações

- Este exemplo é uma base simples para criar páginas e APIs locais.
- É útil para projetos em que a ESP32 atua como cliente da rede e também serve conteúdo via HTTP.

---

## 4) `wifi/examplewifi.ino`

### O que faz

Exemplo básico de conexão Wi-Fi em modo estação.

A ESP32 tenta se conectar à rede informada e imprime o IP local após a conexão.

### Como usar

1. Abra `examples/wifi/examplewifi.ino`.
2. Troque `"SSID"` e `"PASSWORD"` pelos valores da sua rede.
3. Carregue o código na placa.
4. O monitor serial mostrará se a conexão foi bem-sucedida e o IP atribuído.

### Exemplo

```cpp
WiFiJr wifi("SSID", "PASSWORD");

if (wifi.connect()) {
    Serial.println(wifi.getIPAddress());
}
```

### Observações

- Esse é o exemplo mais simples para aprender a biblioteca de Wi-Fi.
- Ele serve como base para projetos que exigem apenas conectividade à internet local.

---

## 5) `wifi/examplewifipointacess.ino`

### O que faz

Esse exemplo cria um ponto de acesso Wi-Fi com a própria ESP32.

Ou seja, a placa funciona como uma rede Wi-Fi aberta para outros dispositivos conectarem.

Ele também inicia um servidor web local para responder requisições HTTP.

### Como usar

1. Abra `examples/wifi/examplewifipointacess.ino`.
2. Ajuste o nome da rede e a senha, se desejar.
3. Faça upload para a ESP32.
4. Conecte seu celular ou notebook na rede criada.
5. Abra a página do servidor usando o endereço IP do Access Point.

### Exemplo de configuração

```cpp
WiFiJr wifi("Teste_Rede_JR", "12345678");
ServerWebJr server(80);
```

### Observações

- Este modo é útil quando você quer uma rede local simples para configurar ou monitorar a placa.
- O IP padrão do Access Point geralmente segue o padrão `192.168.4.1`.

---

## Dicas gerais de uso

- Sempre ajuste o SSID e a senha conforme sua rede real.
- Use o monitor serial para entender o fluxo do programa e diagnosticar falhas de conexão.
- Para projetos com interface web e persistência de dados, combine os exemplos de `wifi` + `serverweb` + `manageFiles`.
- Em projetos mais completos, o exemplo `iot/exampleiot.ino` é o melhor ponto de partida.

## Sequência recomendada para começar

1. Teste `wifi/examplewifi.ino` para verificar conectividade.
2. Teste `serverweb/exampleserverweb.ino` para aprender rotas HTTP.
3. Teste `manageFiles/exampleManageFile.ino` para manipular arquivos persistentes.
4. Use `iot/exampleiot.ino` como base para um projeto IoT completo.

---

## Estrutura de arquivos útil

- `src/` → classes principais da biblioteca.
- `examples/` → sketches prontos para testar e aprender uso.
- `web/` → arquivos HTML que podem ser servidos pela ESP32.

Se quiser, posso também criar um segundo README mais detalhado com diagramas de fluxo e exemplos de montagem para cada caso de uso.
