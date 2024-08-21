# watchdog
watchdog - The purpose of this project is to ensure that a critical process continues to run even after failures. This is done by implementing a watchdog mechanism that monitors the process and restarts it when necessary.

# Usage:

MakeMeImmortal: Call this function with appropriate arguments to start monitoring the process. It will fork a new process to run the watchdog and create a thread to keep an eye on it.
DoNotResuscitate: Call this function to stop the watchdog from restarting the process, effectively stopping the immortality mechanism.

# this project uses:
scheduler - another project I created - it organizes all kinds of tasks and process them at the specified time.

