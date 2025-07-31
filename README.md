# AVIONICS Project

The AVIONICS project is a system designed to collect, process, and log aviation-related data using various sensors and modules (GPS, IMU, BMP180, Bluetooth, etc.). This project is intended to run on embedded systems such as Raspberry Pi.

## Folder Structure and File Descriptions

### src/
This directory contains the source code, including the main application logic, drivers, hardware abstraction layer (HAL), and common utilities.

#### app/
- **Log.c**: Manages log file creation and recording.
  - `LOG_FILE_OPEN()`: Opens the log file in write mode. If the file cannot be opened, it outputs an error and terminates.
  - `LOG_WRITE(const char* data)`: Writes the provided data to the log file.
  - `LOG_WRITE_WITH_TIME(const char* format, ...)`: Writes a formatted log message with a timestamp.
- **Operations.c**: Implements core operational logic.
  - `System_Init()`: Executes system initialization routines.
  - `System_Run()`: Runs the main loop to process sensor data and manage logs.
- **User_CMD_Handler.c**: Handles user commands.
  - `CMD_Process(const char* cmd)`: Parses user input commands and performs appropriate actions.
- **EJC_Handler.c**: Handles specific user commands.
  - `EJC_Init()`: Performs initialization tasks related to EJC.
  - `EJC_Execute(const char* cmd)`: Executes EJC commands.
- **Beacon_Handler.c**: Processes beacon signals.
  - `Beacon_Init()`: Initializes beacon transmission and reception.
  - `Beacon_Process()`: Receives and processes beacon signals.

#### driver/
- **GPS.c**: Handles GPS data collection and processing.
  - `GPS_Init()`: Initializes the GPS module.
  - `GPS_Rx_Handler()`: Receives and parses GPS data.
- **IMU.c**: Processes data from the Inertial Measurement Unit (IMU).
  - `IMU_Init()`: Initializes the IMU module.
  - `IMU_Rx_Handler()`: Receives and processes IMU data.
- **BMP180.c**: Handles data from the barometric pressure sensor.
  - `BMP180_Init()`: Initializes the BMP180 sensor.
  - `BMP180_Read_Pressure()`: Reads and returns pressure data.
- **BT.c**: Manages Bluetooth communication.
  - `BT_Init()`: Initializes the Bluetooth module.
  - `BT_Send(const char* data)`: Sends data via Bluetooth.

#### hal/
- **CPU_Temp.c**: Measures CPU temperature.
  - `Get_CPU_Temp()`: Reads and returns the CPU temperature.
- **GPIO_Handler.c**: Controls GPIO pins.
  - `GPIO_Export(int pin)`: Registers a GPIO pin with the system.
  - `GPIO_Unexport(int pin)`: Unregisters a GPIO pin.
  - `GPIO_Set_Direction(int pin, const char* direction)`: Sets the direction of a GPIO pin.
- **I2C_IO.c**: Handles I2C communication.
  - `I2C_Open(const char* device)`: Opens an I2C port.
  - `I2C_Configure(int fd, int slave_address)`: Configures the I2C slave address.
  - `I2C_Write(int fd, const void* data, size_t length)`: Writes data to the I2C bus.
  - `I2C_Read(int fd, void* buffer, size_t length)`: Reads data from the I2C bus.
  - `I2C_Close(int fd)`: Closes the I2C port.
- **Serial_IO.c**: Handles serial communication.
  - `Serial_Open(const char* device)`: Opens a serial port.
  - `Serial_Write(int fd, const void* data, size_t length)`: Writes data to the serial port.
  - `Serial_Read(int fd, void* buffer, size_t length)`: Reads data from the serial port.
  - `Serial_Close(int fd)`: Closes the serial port.
- **SPI_IO.c**: Handles SPI communication.
  - `SPI_Init(const char* device)`: Initializes the SPI port.
  - `SPI_Transfer(int fd, const void* tx_data, void* rx_data, size_t length)`: Transfers and receives data via SPI.

#### common/
- **BaseDef.c**: Defines basic data types and macros.
- **Time.c**: Provides time-related utility functions.

### build/
Directory for build artifacts and intermediate files.

### log/
Directory for log files generated during execution.

### CMakeLists.txt
CMake build configuration file.

### run.sh
Build and execution script.

## Build and Execution Instructions

1. **Prepare the build folder and build the project**
   ```bash
   ./run.sh
   ```
   The `run.sh` script creates the build directory and uses CMake and Make to build the project.

2. **Create the log directory**
   Ensure the `Log` folder exists to store log files. The script automatically creates it, but you can create it manually if needed:
   ```bash
   mkdir -p Log
   ```

3. **Run the program**
   After building, execute the `run.sh` script to run the program with root privileges. You may need to enter your password.
   ```bash
   bash run.sh
   ```

## Key Features

- **Sensor Data Collection**: Collects real-time data from GPS, IMU, BMP180, and other sensors.
- **Log Management**: Stores data in CSV format in the `Log` directory.
- **Modular Design**: Clearly separates drivers, HAL, and application logic.
- **Extensibility**: Easily add new sensors or modules.

## License

© 2025 Cheolsoon Han. All rights reserved.
