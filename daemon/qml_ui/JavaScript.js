
function sendLocationTo(number)
{
    main.daemonService.sendLocationTo(number);
}

function killDaemonService()
{
    main.daemonService.killDaemon();
}

function enableDaemon(enable)
{
   main.daemonService.enableDaemon(enable);
}

function keepGpsRunning(enable)
{
    main.daemonService.keepGpsRunning(enable);
}

function isDaemonEnabled()
{
    return main.daemonService.isDaemonEnabled();
}

function isDaemonLogEnabled()
{
    return main.daemonService.isLogEnabled();
}

function isGpsRunningEnabled()
{
    return main.daemonService.isGpsRunningEnabled();
}

function getGpsAccuracy()
{
    return main.daemonService.getGpsAccuracy();
}

function setGpsAccuracy(value)
{
    main.daemonService.gpsAccuracy(value);
}

function storeSettings()
{
    main.daemonService.storeSettings();
}

