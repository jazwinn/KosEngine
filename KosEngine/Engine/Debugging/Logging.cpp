/******************************************************************/
/*!
\file      Logging.cpp
\author    Rayner Tan
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     This file contains the implementation of the Logger class,
           which provides logging functionality including error handling,
           signal capturing, and formatted message logging to both
           console and file.

The Logger class supports different log levels, formatted log entries,
signal handling for program crashes, and captures stack traces during
critical events.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../Config/pch.h"
#include "Logging.h"


namespace logging {


    Logger& Logger::m_GetInstance()
    {
        static Logger instance{};
        return instance;
    }
    /******************************************************************/
    /*!
    \fn        Logger::Logger(const std::string& filename)
    \brief     Constructor that opens the log file in append mode.
    \param[in] filename  The name of the file to log messages to.
    */
    /******************************************************************/
    Logger::Logger(const std::string& filename) {
        // Attempt to open the log file in append mode to ensure it exists
#ifdef NO_GAME
        std::ofstream tempFile(filename, std::ios::app);
        if (!tempFile) {
#ifdef IMGUIENABLED
            std::cerr << "Error: Unable to create or access log file: " << filename << std::endl;
#endif
            return;
        }
        tempFile.close(); // Close temp file

        // Open the actual log file for writing (clears existing content)
        m_logFile.open(filename, std::ios::out | std::ios::trunc);
        if (!m_logFile.is_open()) {
#ifdef IMGUIENABLED
            std::cerr << "Error: Failed to open log file for writing: " << filename << std::endl;
#endif
        }
#endif
    }
    /******************************************************************/
    /*!
    \fn        void Logger::m_Setup_Abort_Handler()
    \brief     Sets up signal handlers for various abort and crash signals,
               such as SIGABRT, SIGSEGV, and SIGTERM.
    */
    /******************************************************************/
    void Logger::m_Setup_Abort_Handler() {
#ifdef NO_GAME
        // Handle abort signals
        std::signal(SIGABRT, m_Abort_Handler);

        // Handle segmentation fault (invalid memory access)
        std::signal(SIGSEGV, m_Abort_Handler);

        // Handle floating point exceptions (divide by zero, etc.)
        std::signal(SIGFPE, m_Abort_Handler);

        // Handle illegal instruction exceptions
        std::signal(SIGILL, m_Abort_Handler);

        // Handle termination requests (can be caught for cleanup)
        std::signal(SIGTERM, m_Abort_Handler);
#endif
    }
    /******************************************************************/
    /*!
    \fn        void Logger::m_Abort_Handler(int signal)
    \brief     Handles abort signals and logs the stack trace.
    \param[in] signal The signal that caused the abort, such as SIGABRT,
               SIGSEGV, or SIGTERM.
    */
    /******************************************************************/
    void Logger::m_Abort_Handler(int signal) {
        auto location = std::source_location::current();
        std::string signalDescription{};
        switch (signal) {
        case SIGABRT:
            signalDescription = "SIGABRT: Program aborted";
            break;
        case SIGSEGV:
            signalDescription = "SIGSEGV: Segmentation fault";
            break;
        case SIGFPE:
            signalDescription = "SIGFPE: Floating-point exception";
            break;
        case SIGILL:
            signalDescription = "SIGILL: Illegal instruction";
            break;
        case SIGTERM:
            signalDescription = "SIGTERM: Termination request";
            break;
        default:
            signalDescription = "Unknown signal";
            break;
        }
        LOGGING_CRASH("Abort Signal received: {}",
            signalDescription);

        // Capture the stack trace
        backward::StackTrace st;
        st.load_here(32);

        // Resolve the stack trace to get function, file, and line information
        backward::TraceResolver resolver{};
        resolver.load_stacktrace(st);

        // Log each frame of the stack trace with detailed info
        for (size_t i = 0; i < st.size(); ++i) {
            backward::ResolvedTrace trace = resolver.resolve(st[i]);
            if (!trace.source.filename.empty()) {
                LOGGING_CRASH("Frame {}: Function: {} in file: {} at line: {}", i,
                    trace.source.function,
                    trace.source.filename,
                    trace.source.line);
            }
            else {
                LOGGING_CRASH("Frame {}: Function: {} (unknown source)", i, trace.source.function);
            }
        }

        // Initialize the logger and log stack trace
        auto& logger = logging::Logger::m_GetInstance();
        logger.m_printer.print(st, logger.m_logFile); // Using the printer in Logger to log the stack trace
    }
    /******************************************************************/
    /*!
    \fn        void Logger::m_TestingLog()
    \brief     Logs test messages at different log levels (INFO, DEBUG, ERROR)
               for testing purposes.
    */
    /******************************************************************/
    void Logger::m_TestingLog() {
#ifdef NO_GAME
        LOGGING_INFO("Testing of Logging Information {}" , 50);
        LOGGING_DEBUG("Testing of Logging Debug");
        LOGGING_ERROR("Testing of Logging Error with Source Location");
        LOGGING_ERROR_NO_SOURCE_LOCATION("Testing of Logging without source location");        
#endif
    }
    /******************************************************************/
    /*!
    \fn        Logger::~Logger()
    \brief     Destructor that closes the log file.
    */
    /******************************************************************/
    Logger::~Logger() { 
#ifdef NO_GAME
        m_logFile.close(); 
#endif
    }

    void Logger::m_Init(const std::string& filename) {
#ifdef NO_GAME
        assert(!m_bInitialized && "The logger must be initialized before it is used!");
        if (m_bInitialized)
        {
#ifdef IMGUIENABLED
            std::cout << "The logger must be initialized before it is used!" << std::endl;
#endif
            return;
        }
        else {
#ifdef IMGUIENABLED
            std::cout << "Logger initialized" << std::endl;
#endif
        }

        m_bInitialized = true;

        std::ofstream tempFile(filename, std::ios::app);
        tempFile.close();

        try {
            m_logFile.open(filename, std::ios::out | std::ios::trunc);
            if (!m_logFile.is_open()) {
                throw std::ios_base::failure("Failed to open file: " + filename);
            }
        }
        catch (const std::exception& e) {
            LOGGING_ERROR("Error Init Logging File {}" , e.what());
        }
#endif
    }

    /******************************************************************/
    /*!
    \fn        std::string Logger::m_GetCurrentTimestamp()
    \brief     Gets the current timestamp in the format YYYY-MM-DD HH:MM:SS.
    \return    A string containing the current timestamp.
    */
    /******************************************************************/
    std::string Logger::m_GetCurrentTimestamp() {
        // Get current timestamp
        auto now = std::chrono::system_clock::now();

        // Convert to time_t to manipulate as a calendar time
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        // Convert to tm for local time formatting
        std::tm local_time;
        localtime_s(&local_time, &now_time);

        char buffer[100];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_time);
        
        return std::string(buffer);
    }
    /******************************************************************/
    /*!
    \fn        std::vector<std::string> Logger::m_GetLogList()
    \brief     Returns the list of logs
    */
    /******************************************************************/
    std::vector<std::string> Logger::m_GetLogList() {
        return  m_log_list;
    }

    /******************************************************************/
    /*!
    \fn        void Logger::m_Log(LogLevel level, const std::string& message)
    \brief     Logs a message with the specified log level and outputs it to both
               the console and the log file.
    \param[in] level   The severity level of the log (INFO, DEBUG, WARNING, ERROR).
    \param[in] message The log message to be logged.
    */
    /******************************************************************/
    void Logger::m_Log(LogLevel level, const std::string& message)
    {
#ifdef NO_GAME
        std::string current_Time = m_GetCurrentTimestamp();
        // Create log entry
        std::ostringstream logEntry;
        logEntry << "[" << current_Time << "] " << m_LevelToString(level) << ": " << message << "\n";

        // Output to console
#ifdef IMGUIENABLED
        std::cout << m_ColorToString(level) << logEntry.str() << s_CLOSE << std::endl;
#endif
        m_log_list.push_back(logEntry.str());
        // Output to log file
        if (m_logFile.is_open()) {
            m_logFile << logEntry.str();
            m_logFile.flush(); // Ensure immediate write to file
        }

#endif
    }

    /******************************************************************/
    /*!
    \fn        std::string Logger::m_LevelToString(LogLevel level)
    \brief     Converts the log level enum to its corresponding string representation.
    \param[in] level  The log level to be converted.
    \return    The string representation of the log level.
    */
    /******************************************************************/
    std::string Logger::m_LevelToString(LogLevel level)
    {
        switch (level) {
        case LogLevel::LOG_DEBUG:
            return "DEBUG";
        case LogLevel::LOG_INFO:
            return "INFO";
        case LogLevel::LOG_WARNING:
            return "WARNING";
        case LogLevel::LOG_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }
    /******************************************************************/
    /*!
    \fn        std::string Logger::m_ColorToString(LogLevel level)
    \brief     Converts the log level to a color-coded string for console output.
    \param[in] level  The log level to be converted to a color code.
    \return    The color-coded string representing the log level.
    */
    /******************************************************************/
    std::string Logger::m_ColorToString(LogLevel level) {
        switch (level) {
        case LogLevel::LOG_DEBUG: //GREEN
            return s_GREEN;
        case LogLevel::LOG_INFO:  //WHITE
            return s_WHITE;
        case LogLevel::LOG_WARNING: //YELLOW
            return s_YELLOW;
        case LogLevel::LOG_ERROR:   //RED
            return s_RED;
        default:
            return s_CLOSE;
        }
    }    
}
