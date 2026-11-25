# Integração ESP32 com Home Assistant utilizando MQTT

Este documento descreve como preparar o **Home Assistant** para receber
dados publicados por um ESP32 por meio do protocolo MQTT. O exemplo de
aplicação considerado é um sensor ultrassônico de distância, mas a
configuração pode ser reutilizada para outros sensores.

## 1. Arquitetura

A comunicação utiliza três componentes:

-   **ESP32:** mede os dados e publica mensagens MQTT pela rede Wi-Fi.
-   **Mosquitto Broker:** recebe e distribui as mensagens MQTT.
-   **Home Assistant:** recebe os dados e os disponibiliza como
    entidades para dashboards, gráficos e automações.

Fluxo de comunicação:

``` text
Sensor → ESP32 → Wi-Fi → Mosquitto Broker → Home Assistant
```

O ESP32 e o broker precisam ter conectividade de rede entre si. A porta
padrão do MQTT sem TLS é **1883**.

## 2. Identificar o endereço IP do servidor

1.  Acesse a interface web do Home Assistant.
2.  Abra **Configurações → Sistema → Rede**.
3.  Identifique o endereço IPv4 da interface de rede utilizada pelo
    servidor.
4.  Anote o endereço. Ele será usado como endereço do broker caso o
    Mosquitto esteja instalado na mesma máquina ou ambiente de rede.

Exemplo ilustrativo: `192.168.1.100`.

> O endereço acima é apenas um exemplo. Utilize o IP real da sua
> instalação. Se o broker estiver em outra máquina, use o IP dessa
> máquina, não necessariamente o IP do Home Assistant.

É recomendável configurar uma reserva DHCP no roteador ou um endereço
estático para o servidor, evitando que o IP do broker mude.

## 3. Instalar o Mosquitto Broker

Em instalações do Home Assistant OS que oferecem a loja de aplicativos:

1.  Abra **Configurações → Aplicativos** (em versões anteriores, a seção
    pode aparecer como **Add-ons**).
2.  Abra a loja de aplicativos.
3.  Procure por **Mosquitto broker**, o aplicativo oficial.
4.  Abra a página do aplicativo e selecione **Instalar**.
5.  Após a instalação, abra a página de configuração.
6.  Inicie o serviço.
7.  Se disponíveis, habilite as opções **Iniciar na inicialização** e
    **Watchdog**.

Depois, consulte a aba **Log** e confirme que o broker iniciou sem
erros.

> A disponibilidade de aplicativos/add-ons depende do tipo de instalação
> do Home Assistant. Em instalações Container ou Core, a loja de
> aplicativos não está disponível; nesses casos, o Mosquitto deve ser
> instalado e administrado separadamente.

## 4. Criar credenciais para o ESP32

Crie um usuário dedicado para a conexão MQTT:

1.  Acesse **Configurações → Pessoas**.
2.  Abra a seção **Usuários**.
3.  Selecione **Adicionar usuário**.
4.  Crie um usuário, por exemplo, `esp32`.
5.  Defina uma senha forte.
6.  Não conceda privilégios de administrador.
7.  Salve as alterações.

Use essas credenciais na configuração do cliente MQTT do ESP32.

**Importante:** não utilize nomes reservados pelo aplicativo Mosquitto,
como `homeassistant` ou `addons`, para esse usuário.

## 5. Configurar a integração MQTT no Home Assistant

1.  Acesse **Configurações → Dispositivos e serviços**.
2.  Se a integração MQTT ainda não estiver configurada, selecione
    **Adicionar integração**.
3.  Pesquise por **MQTT** e selecione a integração.
4.  Se o broker for descoberto automaticamente, confira os dados e
    conclua a configuração.
5.  Se for solicitada configuração manual, informe o endereço do broker,
    a porta `1883` e as credenciais configuradas.

Exemplo de parâmetros:

  Parâmetro   Exemplo
  ----------- -------------------------------
  Broker      `192.168.1.100`
  Porta       `1883`
  Usuário     `esp32`
  Senha       A senha criada para o usuário

O endereço e as credenciais são exemplos e devem ser substituídos pelos
valores da instalação.

## 6. Testar o recebimento de mensagens MQTT

Antes de configurar uma entidade, teste se o Home Assistant recebe
mensagens do ESP32:

1.  Abra **Configurações → Dispositivos e serviços**.

2.  Selecione a integração **MQTT**.

3.  Abra a opção de configurar e localize a ferramenta para escutar um
    tópico (*Listen to a topic*).

4.  Informe o tópico:

    `casa/esp32/distancia`

5.  Inicie a escuta.

6.  Ligue o ESP32 e aguarde as publicações.

Se a comunicação estiver correta, a ferramenta mostrará as mensagens
recebidas nesse tópico, por exemplo:

``` text
Tópico: casa/esp32/distancia
Payload: 25.40
```

O valor é ilustrativo. O payload deve ser um valor numérico em texto,
expresso em centímetros.

Se nenhuma mensagem aparecer, verifique:

-   Se o ESP32 está conectado ao Wi-Fi.
-   Se o IP e a porta do broker estão corretos.
-   Se usuário e senha estão corretos.
-   Se o Mosquitto está em execução.
-   Se o ESP32 consegue alcançar o servidor pela rede.
-   Se o tópico observado é exatamente igual ao tópico publicado.

## 7. Criar a entidade de distância

Há duas formas comuns de criar sensores MQTT no Home Assistant: pela
configuração YAML ou por descoberta MQTT (MQTT Discovery). Para este
procedimento, será utilizada a configuração YAML manual.

### 7.1. Abrir o arquivo de configuração

Abra o arquivo `configuration.yaml` usando um editor de arquivos
disponível na sua instalação.

### 7.2. Adicionar a configuração

Inclua:

``` yaml
mqtt:
  sensor:
    - name: "Distância Ultrassônica"
      unique_id: "esp32_distancia"
      state_topic: "casa/esp32/distancia"
      unit_of_measurement: "cm"
      device_class: distance
      state_class: measurement
```

Se o arquivo já tiver uma seção `mqtt:`, não crie uma segunda seção com
o mesmo nome. Adicione o bloco `sensor:` dentro da seção existente,
respeitando a indentação YAML.

Essa configuração informa ao Home Assistant:

  Campo                   Finalidade
  ----------------------- -------------------------------------------
  `name`                  Nome apresentado para o sensor
  `unique_id`             Identificador único da entidade
  `state_topic`           Tópico MQTT que contém a leitura
  `unit_of_measurement`   Unidade da leitura, centímetros
  `device_class`          Classifica a medição como distância
  `state_class`           Permite registrar estatísticas de medição

### 7.3. Validar e reiniciar

1.  Salve o arquivo.
2.  Acesse **Ferramentas do desenvolvedor → YAML**.
3.  Execute a verificação de configuração, se essa opção estiver
    disponível.
4.  Se não houver erros, reinicie o Home Assistant para aplicar a
    configuração.

## 8. Verificar a entidade

Após a reinicialização:

1.  Abra **Ferramentas do desenvolvedor → Estados**.

2.  Pesquise pela entidade:

    `sensor.distancia_ultrassonica`

3.  Verifique se o estado apresenta o último valor recebido.

Exemplo:

``` text
Entidade: sensor.distancia_ultrassonica
Estado: 25.40

Atributos:
  unit_of_measurement: cm
  device_class: distance
  state_class: measurement
```

A entidade só terá um estado válido depois que o Home Assistant receber
uma mensagem no tópico configurado.

## 9. Adicionar a medição ao dashboard

1.  Abra o dashboard desejado.
2.  Entre no modo de edição.
3.  Adicione um cartão **Histórico** ou outro cartão compatível.
4.  Selecione `sensor.distancia_ultrassonica`.
5.  Salve o dashboard.

O histórico dependerá de o Home Assistant estar recebendo atualizações e
mantendo o histórico da entidade.

## 10. Organização dos tópicos para expansão

Se o projeto receber outros sensores ou ESP32, mantenha uma convenção
consistente para os tópicos. Por exemplo:

``` text
casa/
├── esp32_01/
│   ├── distancia
│   ├── temperatura
│   └── umidade
├── esp32_02/
│   ├── luminosidade
│   └── temperatura
└── esp32_03/
    └── nivel_agua
```

Cada entidade MQTT do Home Assistant deve apontar para o tópico
correspondente.

## 11. Segurança e recomendações

-   Use um usuário MQTT exclusivo para cada aplicação ou grupo de
    dispositivos, quando possível.
-   Utilize senhas fortes e não as publique em repositórios de código.
-   A porta `1883` não oferece criptografia TLS. Restrinja o acesso ao
    broker à rede confiável.
-   Para comunicação fora de uma rede confiável, configure TLS e as
    regras de firewall apropriadas.
-   Evite expor a porta MQTT diretamente à internet.
-   Considere configurar uma reserva DHCP para o servidor do broker.
-   Em aplicações maiores, avalie MQTT Discovery para cadastrar
    entidades automaticamente, sem editar manualmente o
    `configuration.yaml` para cada sensor.

## 12. Referências oficiais

-   [Integração MQTT do Home
    Assistant](https://www.home-assistant.io/integrations/mqtt/)
-   [Sensor MQTT do Home
    Assistant](https://www.home-assistant.io/integrations/sensor.mqtt/)
-   [Aplicativo Mosquitto Broker para Home
    Assistant](https://github.com/home-assistant-apps/mosquitto)
-   [Documentação do Mosquitto](https://mosquitto.org/documentation/)

------------------------------------------------------------------------

**Resultado esperado:** o Mosquitto recebe as mensagens publicadas pelo
ESP32 no tópico `casa/esp32/distancia`, e o Home Assistant disponibiliza
essas leituras na entidade `sensor.distancia_ultrassonica`, em
centímetros.
