watchdogd
==========

Watchdog Daemon process that feeds the Watchdog device at a given rate.
Based on Watchdogd from AOSP source code at:
https://github.com/TuringComputer/platform_system_core/blob/imx6x_lollipop/init/watchdogd.c

Usage:
Set watchdog timer to 30 seconds and pet it every 10 seconds to get a 20 second margin:
watchdogd 10 20
