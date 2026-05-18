# 📡 Sistema IoT - Sensor de Luz con ESP32 y Dashboard Web

Sistema completo de monitoreo de luz en tiempo real usando ESP32, sensor LDR y un dashboard web alojado en AWS.

## 🎯 Características

- ✅ Lectura del sensor de luz LDR (digital y analógico)
- ✅ Envío de datos por WiFi a servidor en AWS
- ✅ Dashboard web con gráfica en tiempo real
- ✅ Historial con filtros por fecha y hora
- ✅ Clasificación automática del nivel de luz
- ✅ Control remoto: pausar / reanudar / Deep Sleep
- ✅ Apagado físico del sensor mediante GPIO
- ✅ Reconexión automática a WiFi
- ✅ Hora local de Perú (UTC-5)

## 🛠️ Hardware necesario

- ESP32 Dev Module
- Sensor de luz LDR (módulo de 4 pines: VCC, GND, DO, AO)
- Protoboard
- Cables jumper
- Cable USB

## 🔌 Conexiones

| Sensor LDR | ESP32 |
|------------|-------|
| VCC        | GPIO 26 (pin de control de energía) |
| GND        | GND |
| DO         | GPIO 14 (salida digital) |
| AO         | GPIO 34 (salida analógica) |

## 📋 Software requerido

### En el ESP32
- Arduino IDE 2.x
- Paquete ESP32 by Espressif Systems (v2.0.17)
- Librerías: WiFi, HTTPClient

### En el servidor AWS
- Node.js v20+
- Express
- Ubuntu 22.04 (instancia EC2)

## ⚙️ Instalación

### 1. Servidor AWS

```bash
git clone https://github.com/Aracely271004/sensor-luz-esp32.git
cd sensor-luz-esp32
npm install express
node server.js
```

Abre el puerto **4000** en el grupo de seguridad de EC2.

### 2. ESP32

1. Abre `esp32/Sensor_de_luz_AWS.ino` en Arduino IDE
2. Modifica las variables al inicio:
```cpp
   const char* ssid     = "TU_WIFI";
   const char* password = "TU_PASSWORD";
   const char* urlDatos = "http://TU_IP_AWS:4000/datos";
```
3. Selecciona placa: **ESP32 Dev Module**
4. Sube el código

## 🌐 Uso

Accede al dashboard en:
```
http://TU_IP_AWS:4000
```

### Niveles de luz
- 🔘 **No hay luz** — analógico < 500
- 🟠 **Poca luz** — 500 a 1499
- 🟢 **Luz regular** — 1500 a 2799
- 🟡 **Mucha luz** — 2800 a 4095

### Controles disponibles
- **⏸ Pausar Sensor** — detiene el envío de datos
- **😴 Modo Deep Sleep** — apaga el ESP32 con bajo consumo

## 📂 Estructura del proyecto

```
sensor-luz-esp32/
├── server.js              # Backend Node.js
├── index.html             # Dashboard web
├── package.json           # Dependencias
├── README.md              # Este archivo
└── esp32/
    └── Sensor_de_luz_AWS.ino  # Código del ESP32
```

## 📊 API Endpoints

| Método | Ruta | Descripción |
|--------|------|-------------|
| POST   | `/datos`   | Recibe lectura del sensor |
| GET    | `/datos`   | Devuelve historial (filtros: ?desde&hasta) |
| GET    | `/estado`  | Estado del sensor (activo/pausado) |
| POST   | `/estado`  | Cambia el estado |
| GET    | `/comando` | Comando actual (dormir/despierto) |
| POST   | `/comando` | Cambia el comando |

## 👤 Autor

Aracely Fiorela Corampa Palacios

## 📝 Licencia

MIT License
