![](https://runelite.net/img/logo.png)
# launcher [![Discord](https://img.shields.io/discord/301497432909414422.svg)](https://discord.gg/mePCs8U)

This repository holds the launcher for the [RuneLite client](https://github.com/runelite/runelite).

If you have any questions, please join our [Discord](https://discord.gg/mePCs8U) server or alternatively our IRC channel on [irc.rizon.net #runelite](http://qchat.rizon.net/?channels=runelite&uio=d4).

### License

RuneLite is [licensed under the BSD 2-clause license](https://github.com/runelite/launcher/blob/master/LICENSE).

## Behavior of isReadable/Writable/Executable vs canRead/Write/Execute in different scenarios

### Conclusions
Conclusions are as expected:
- If a folder or file does not exist, all methods return false.
- A folder can't really be set to read-only via the GUI in windows, a file can. isWritable and canWrite will return false for a file with a read-only attribute, regardless of running with or without elevated permissions.
- If permissions are removed using icacls, then canRead/Write/Execute still return true$! They only check the file attributes as far as I'm aware, not the ACL. isReadable/Writable/Executable does properly return false though (when not running elevated).

### Removing file perms
File perms have been removed for the local user with icacls to simulate the behavior as seen in ```#support```

__Initial permissions__

``icacls "%userprofile%\.runelite\testfolder\FilePermsVerification"``
```
C:\Users\LauncherTest\.runelite\testfolder\FilePermsVerification NT AUTHORITY\SYSTEM:(F)
                                                                 BUILTIN\Administrators:(F)
                                                                 DESKTOP-QSUTV7K\LauncherTest:(F)
```


``icacls "%userprofile%\.runelite\testfolder"``
```
C:\Users\LauncherTest\.runelite\testfolder NT AUTHORITY\SYSTEM:(OI)(CI)(F)
                                           BUILTIN\Administrators:(OI)(CI)(F)
                                           DESKTOP-QSUTV7K\LauncherTest:(OI)(CI)(F)
```

__Removing permissions__
```
icacls "%userprofile%\.runelite\testfolder\FilePermsVerification" /remove:g DESKTOP-QSUTV7K\LauncherTest /inheritance:r /t /c
```
which results in:
```
C:\Users\LauncherTest\.runelite\testfolder\FilePermsVerification NT AUTHORITY\SYSTEM:(F)
                                                                 BUILTIN\Administrators:(F)
```

```
icacls "%userprofile%\.runelite\testfolder" /remove:g DESKTOP-QSUTV7K\LauncherTest /inheritance:r /t /c
```
which results in:
```
C:\Users\LauncherTest\.runelite\testfolder NT AUTHORITY\SYSTEM:(OI)(CI)(F)
                                           BUILTIN\Administrators:(OI)(CI)(F)
```

and maybe that command was a bit too agressive because it also results in:
```
C:\Users\LauncherTest\.runelite\testfolder\FilePermsVerification
Successfully processed 1 files; Failed processing 0 files
```
$ = Aka no permissions at all it seems like, but it should still be fine for testing. This might have been a bit too much, so canRead/Write/Execute did also return false for the file (not for the folder though!) in this case. I expect them to incorrectly return true if some of the permissions had remained for the file.

### Test results

**No permission problems**

__Not elevated__

Folder doesn't exist, file doesn't exist
```
Launcher is running with elevated permissions: false
2024-03-02 04:22:30 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): false
2024-03-02 04:22:30 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): false
2024-03-02 04:22:30 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): false
2024-03-02 04:22:30 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): false
2024-03-02 04:22:30 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): false
2024-03-02 04:22:30 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): false
2024-03-02 04:22:30 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): false
2024-03-02 04:22:30 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 04:22:30 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): false
2024-03-02 04:22:30 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): false
2024-03-02 04:22:30 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 04:22:30 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): false
```

Folder exists, file does not exist
```
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: false
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): false
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): false
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): false
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 04:29:25 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): false
```

Folder exists, file exists
```
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: false
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): true
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): true
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): true
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): true
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): true
2024-03-02 04:31:11 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): true
```

__Elevated__

Folder doesn't exist, file doesn't exist
```
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: true
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): false
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): false
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): false
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): false
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): false
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): false
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): false
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): false
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): false
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 04:34:40 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): false
```

Folder exists, file does not exist
```
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: true
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): false
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): false
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): false
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 04:33:21 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): false
```

Folder exists, file exists
```
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: true
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): true
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): true
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): true
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): true
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): true
2024-03-02 04:32:13 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): true
```

**Read only attribute file**

__Not elevated__

Folder exists, file exists
```
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: false
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): true
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): true
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): true
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 04:41:51 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): true
```

__Elevated__

Folder exists, file exists
```
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: true
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): true
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): true
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): true
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 04:42:39 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): true
```

**Read only attribute folder (only applies to files in folder according to windows and does actually set the property for the file inside the folder, so probs not folder itself)**

__Not elevated__

Folder exists, file does not exist
```
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: false
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): false
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): false
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): false
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 04:54:10 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): false
```

Folder exists, file exists
```
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: false
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): true
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): true
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): true
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 04:52:07 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): true
```

__Elevated__

Folder exists, file does not exist
```
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: true
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): false
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): false
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): false
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 04:55:00 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): false
```

Folder exists, file exists
```
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: true
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): true
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): true
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): true
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 04:52:46 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): true
```

**File permissions removed via icacls**

__Not elevated__

Folder exists, file exists
```
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: false
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): false
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): false
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): true
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): true
2024-03-02 05:17:10 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): true
```

__Elevated__

Folder exists, file exists
```
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: true
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): true
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): true
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): true
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): true
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): true
2024-03-02 05:19:28 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): true
```

**Folder permissions removed via icacls**

__Not elevated__

Folder exists, file does not exist
```
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: false
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): false
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): false
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): false
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): false
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): false
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): false
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 05:55:02 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): false
```

Folder exists, file exists
```
2024-03-02 05:30:56 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): false
2024-03-02 05:30:56 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): false
2024-03-02 05:30:56 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): false
2024-03-02 05:30:56 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 05:30:56 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 05:30:56 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 05:30:56 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): false
2024-03-02 05:30:56 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 05:30:56 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): false
2024-03-02 05:30:56 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): false
2024-03-02 05:30:56 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 05:30:56 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): false
```

__Elevated__

Folder exists, file does not exist
```
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: true
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): false
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): false
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): false
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): false
2024-03-02 05:57:36 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): false
```

Folder exists, file exists
```
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - Launcher is running with elevated permissions: true
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFOLDER.toPath()): true
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFOLDER.toPath()): true
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFOLDER.toPath()): true
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canRead(): true
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canWrite(): true
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - TESTFOLDER.canExecute(): true
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - Files.isReadable(TESTFILE.toPath()): false
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - Files.isWritable(TESTFILE.toPath()): false
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - Files.isExecutable(TESTFILE.toPath()): false
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canRead(): true
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canWrite(): true
2024-03-02 05:33:08 CET [main] INFO  net.runelite.launcher.Launcher - TESTFILE.canExecute(): true
```
