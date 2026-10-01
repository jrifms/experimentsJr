# Scripts de upload e geração de SPIFFS

Esta pasta contém dois scripts usados para preparar e gravar o sistema de arquivos SPIFFS em dispositivos ESP.

## Estrutura

- `gerar_spiff.sh` — gera o arquivo `spiffs.bin` a partir da pasta `data/`.
- `enviar.sh` — grava o arquivo `spiffs.bin` na memória flash do dispositivo.

> Observação: os caminhos e comandos no arquivo foram escritos para ambiente Linux e apontam para ferramentas instaladas pela plataforma ESP8266. A mesma ideia funciona para ESP32, mas é necessário ajustar o executável, a porta serial e o offset da partição.

---

## 1) Script `gerar_spiff.sh`

### O que faz

Esse script gera um arquivo binário para o SPIFFS usando o utilitário `mkspiffs`.

Conteúdo atual:

```bash
/home/jr/.arduino15/packages/esp8266/tools/mkspiffs/3.1.0-gcc10.3-e5f9fec/mkspiffs \
-c data \
-p 256 \
-b 8192 \
-s 0x200000 \
spiffs.bin
```

### Significado dos parâmetros

- `-c data` → usa a pasta `data` como origem dos arquivos.
- `-p 256` → define o tamanho do bloco de página.
- `-b 8192` → tamanho do bloco de dados.
- `-s 0x200000` → tamanho total da imagem SPIFFS.
- `spiffs.bin` → arquivo gerado.

### Como usar

1. Crie uma pasta chamada `data` no diretório do projeto.
2. Coloque os arquivos HTML, CSS, JSON, imagens etc. dentro dela.
3. Execute:

```bash
chmod +x gerar_spiff.sh
./gerar_spiff.sh
```

### Para ESP32

No ESP32 o processo é o mesmo, porém normalmente a geração é feita com `mkspiffs` do pacote da placa ESP32. O comando equivalente em ambiente Windows pode ser:

```powershell
mkspiffs.exe -c data -p 256 -b 8192 -s 0x200000 spiffs.bin
```

---

## 2) Script `enviar.sh`

### O que faz

Esse script grava o arquivo `spiffs.bin` diretamente na flash do dispositivo via `esptool`.

Conteúdo atual:

```bash
/home/jr/.arduino15/packages/esp8266/hardware/esp8266/3.1.2/tools/upload.py \
--chip esp8266 \
--port /dev/ttyUSB0 \
--baud 115200 \
write_flash \
0x200000 \
spiffs.bin
```

### Significado dos parâmetros

- `--chip esp8266` → modelo da placa.
- `--port /dev/ttyUSB0` → porta serial do dispositivo no Linux.
- `--baud 115200` → velocidade de comunicação.
- `write_flash` → comando para gravar na flash.
- `0x200000` → endereço de memória da partição SPIFFS.
- `spiffs.bin` → arquivo a ser gravado.

### Como usar

1. Gere o arquivo `spiffs.bin` com `gerar_spiff.sh`.
2. Conecte a ESP ao computador.
3. Execute:

```bash
chmod +x enviar.sh
./enviar.sh
```

> O script assume que a placa é ESP8266 e usa a porta `/dev/ttyUSB0`. Se a porta for outra, ajuste o valor.

---

## Como fazer a mesma operação na ESP32

A lógica é a mesma: gerar uma imagem de arquivos e gravá-la no flash da ESP32, mas com diferença no comando e no offset da partição.

### Opção 1: usando ferramentas do Arduino / ESP32

Se você estiver usando a ESP32 no Arduino IDE, a forma mais simples é:

1. Crie a pasta `data` com os arquivos do site ou da interface.
2. Use o utilitário `mkspiffs` do pacote ESP32.
3. Gere o arquivo `spiffs.bin`.
4. Grave via `esptool.py` ou via ferramenta do IDE, conforme o chipset e o esquema de partição.

### Opção 2: usando `esptool.py`

Exemplo geral:

```bash
python -m esptool --chip esp32 --port /dev/ttyUSB0 --baud 115200 write_flash 0x290000 spiffs.bin
```

Ou no Windows:

```powershell
esptool.exe --chip esp32 --port COM3 --baud 115200 write_flash 0x290000 spiffs.bin
```

### Importante sobre o offset

O endereço `0x290000` pode variar dependendo da partição utilizada.

Os valores típicos incluem:

- `0x290000` → para parte de dados em placas com memória maior e partição padrão;
- `0x100000` ou outros offsets → dependendo do esquema de partição.

Para evitar erro, consulte o mapa da partição da sua placa no Arduino IDE:

- `Ferramentas > Partição`
- `Ferramentas > Porta`
- `Ferramentas > Velocidade`

O offset ideal deve combinar com o esquema de partição de arquivos usado pela sua ESP32.

---

## Procedimento no ambiente Windows

No Windows, o fluxo recomendado é:

### 1) Instalar o suporte da ESP32 no Arduino IDE

1. Abra o Arduino IDE.
2. Vá em `Arquivo > Preferências`.
3. Em "URLs adicionais de gerenciador de placas", adicione:

```text
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

4. Vá em `Ferramentas > Placa > Gerenciador de placas`.
5. Procure por `esp32` e instale o pacote da Espressif.

### 2) Instalar o `esptool` e `mkspiffs`

A maneira mais simples é usar a instalação do Arduino IDE ou o Python com `pip`:

```powershell
pip install esptool
```

Se preferir usar os binários da instalação do Arduino, procure em:

```text
C:\Users\<seu_usuario>\AppData\Local\Arduino15\packages\esp32\tools\
```

### 3) Criar a pasta `data`

Crie uma pasta chamada `data` dentro do projeto e coloque nela os arquivos que a ESP32 vai servir ou ler.

### 4) Gerar o arquivo SPIFFS

No PowerShell ou CMD:

```powershell
mkspiffs.exe -c data -p 256 -b 8192 -s 0x200000 spiffs.bin
```

### 5) Verificar a porta COM

No Windows, a porta geralmente aparece como `COM3`, `COM4`, etc.

Você pode confirmar em:

- `Painel de Controle > Hardware e Sons > Dispositivos e Impressoras`
- ou no Arduino IDE em `Ferramentas > Porta`

### 6) Gravar na ESP32

```powershell
esptool.exe --chip esp32 --port COM3 --baud 115200 write_flash 0x290000 spiffs.bin
```

Se a porta for diferente, troque `COM3` pelo valor correto.

---

## Dica prática para projetos com ESP32

Se o objetivo for apenas usar arquivos HTML, CSS ou JSON pela ESP32, muitas pessoas preferem o caminho mais simples:

- colocar os arquivos em `data/`;
- gerar `spiffs.bin`; 
- gravar na flash com `esptool`;
- reiniciar a placa.

Isso funciona muito bem para páginas web, configurações locais e armazenamento leve.

---

## Observação final

Os scripts da pasta `scritps` foram escritos com foco em Linux e em placas ESP8266, mas a ideia é idêntica para ESP32. O principal ajuste é o comando e o endereço de gravação da partição, além da porta serial correta.

Se quiser, posso também criar uma versão destes scripts já adaptada para ESP32 e para Windows, em formato `.bat` ou `.ps1`.
