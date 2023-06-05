// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "OptionsParser.h"
#include <QDir>

namespace android {
namespace deploy {

bool OptionsParser::parseOptions(QStringList arguments)
{
    for (int i=0; i<arguments.size(); ++i) {
        const QString &argument = arguments.at(i);
        if (argument.compare("--output"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.outputDirectory = arguments.at(++i).trimmed();
        } else if (argument.compare("--input"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.inputFileName = arguments.at(++i);
        } else if (argument.compare("--aab"_L1, Qt::CaseInsensitive) == 0) {
            options.buildAAB = true;
            options.build = true;
        } else if (!options.buildAAB && argument.compare("--no-build"_L1, Qt::CaseInsensitive) == 0) {
            options.build = false;
        } else if (argument.compare("--install"_L1, Qt::CaseInsensitive) == 0) {
            options.installApk = true;
            options.uninstallApk = true;
        } else if (argument.compare("--reinstall"_L1, Qt::CaseInsensitive) == 0) {
            options.installApk = true;
            options.uninstallApk = false;
        } else if (argument.compare("--android-platform"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.androidPlatform = arguments.at(++i);
        } else if (argument.compare("--help"_L1, Qt::CaseInsensitive) == 0) {
            options.helpRequested = true;
        } else if (argument.compare("--verbose"_L1, Qt::CaseInsensitive) == 0) {
            options.verbose = true;
        } else if (argument.compare("--deployment"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size()) {
                options.helpRequested = true;
            } else {
                QString deploymentMechanism = arguments.at(++i);
                if (deploymentMechanism.compare("bundled"_L1, Qt::CaseInsensitive) == 0) {
                    options.deploymentMechanism = Options::Bundled;
                } else if (deploymentMechanism.compare("unbundled"_L1,
                                                       Qt::CaseInsensitive) == 0) {
                    options.deploymentMechanism = Options::Unbundled;
                } else {
                    fprintf(stderr, "Unrecognized deployment mechanism: %s\n", qPrintable(deploymentMechanism));
                    options.helpRequested = true;
                }
            }
        } else if (argument.compare("--device"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.installLocation = arguments.at(++i);
        } else if (argument.compare("--release"_L1, Qt::CaseInsensitive) == 0) {
            options.releasePackage = true;
        } else if (argument.compare("--jdk"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.jdkPath = arguments.at(++i);
        } else if (argument.compare("--apk"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.apkPath = arguments.at(++i);
        } else if (argument.compare("--depfile"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.depFilePath = arguments.at(++i);
        } else if (argument.compare("--builddir"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.buildDirectory = arguments.at(++i);
        } else if (argument.compare("--sign"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 2 >= arguments.size()) {
                const QString keyStore = qEnvironmentVariable("QT_ANDROID_KEYSTORE_PATH");
                const QString storeAlias = qEnvironmentVariable("QT_ANDROID_KEYSTORE_ALIAS");
                if (keyStore.isEmpty() || storeAlias.isEmpty()) {
                    options.helpRequested = true;
                    fprintf(stderr, "Package signing path and alias values are not specified.\n");
                } else {
                    fprintf(stdout,
                            "Using package signing path and alias values found from the "
                            "environment variables.\n");
                    options.keyStore = keyStore;
                    options.keyStoreAlias = storeAlias;
                }
            } else if (!arguments.at(i + 1).startsWith("--"_L1) &&
                       !arguments.at(i + 2).startsWith("--"_L1)) {
                options.keyStore = arguments.at(++i);
                options.keyStoreAlias = arguments.at(++i);
            } else {
                options.helpRequested = true;
                fprintf(stderr, "Package signing path and alias values are not "
                                "specified.\n");
            }

            // Do not override if the passwords are provided through arguments
            if (options.keyStorePassword.isEmpty()) {
                fprintf(stdout, "Using package signing store password found from the environment "
                                "variable.\n");
                options.keyStorePassword = qEnvironmentVariable("QT_ANDROID_KEYSTORE_STORE_PASS");
            }
            if (options.keyPass.isEmpty()) {
                fprintf(stdout, "Using package signing key password found from the environment "
                                "variable.\n");
                options.keyPass = qEnvironmentVariable("QT_ANDROID_KEYSTORE_KEY_PASS");
            }
        } else if (argument.compare("--storepass"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.keyStorePassword = arguments.at(++i);
        } else if (argument.compare("--storetype"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.storeType = arguments.at(++i);
        } else if (argument.compare("--keypass"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.keyPass = arguments.at(++i);
        } else if (argument.compare("--sigfile"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.sigFile = arguments.at(++i);
        } else if (argument.compare("--digestalg"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.digestAlg = arguments.at(++i);
        } else if (argument.compare("--sigalg"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.sigAlg = arguments.at(++i);
        } else if (argument.compare("--tsa"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.tsaUrl = arguments.at(++i);
        } else if (argument.compare("--tsacert"_L1, Qt::CaseInsensitive) == 0) {
            if (i + 1 == arguments.size())
                options.helpRequested = true;
            else
                options.tsaCert = arguments.at(++i);
        } else if (argument.compare("--internalsf"_L1, Qt::CaseInsensitive) == 0) {
            options.internalSf = true;
        } else if (argument.compare("--sectionsonly"_L1, Qt::CaseInsensitive) == 0) {
            options.sectionsOnly = true;
        } else if (argument.compare("--protected"_L1, Qt::CaseInsensitive) == 0) {
            options.protectedAuthenticationPath = true;
        } else if (argument.compare("--aux-mode"_L1, Qt::CaseInsensitive) == 0) {
            options.auxMode = true;
        } else if (argument.compare("--qml-importscanner-binary"_L1, Qt::CaseInsensitive) == 0) {
            options.qmlImportScannerBinaryPath = arguments.at(++i).trimmed();
        } else if (argument.compare("--no-rcc-bundle-cleanup"_L1,
                                    Qt::CaseInsensitive) == 0) {
            options.noRccBundleCleanup = true;
        } else if (argument.compare("--copy-dependencies-only"_L1,
                                    Qt::CaseInsensitive) == 0) {
            options.copyDependenciesOnly = true;
        }
    }

    if (options.buildDirectory.isEmpty() && !options.depFilePath.isEmpty())
        options.helpRequested = true;

    if (options.inputFileName.isEmpty())
        options.inputFileName = "android-%1-deployment-settings.json"_L1.arg(QDir::current().dirName());

    options.timing = qEnvironmentVariableIsSet("ANDROIDDEPLOYQT_TIMING_OUTPUT");

    if (!QDir::current().mkpath(options.outputDirectory)) {
        fprintf(stderr, "Invalid output directory: %s\n", qPrintable(options.outputDirectory));
        options.outputDirectory.clear();
    } else {
        options.outputDirectory = QFileInfo(options.outputDirectory).canonicalFilePath();
        if (!options.outputDirectory.endsWith(u'/'))
            options.outputDirectory += u'/';
    }

    return true;
}

void OptionsParser::printHelp() const {
    fprintf(stderr, R"(
Syntax: androiddeployqt --output <destination> [options]

Creates an Android package in the build directory <destination> and
builds it into an .apk file.

Optional arguments:
    --input <inputfile>: Reads <inputfile> for options generated by
       qmake. A default file name based on the current working
       directory will be used if nothing else is specified.

    --deployment <mechanism>: Supported deployment mechanisms:
       bundled (default): Includes Qt files in stand-alone package.
       unbundled: Assumes native libraries are present on the device
       and does not include them in the APK.

    --aab: Build an Android App Bundle.

    --no-build: Do not build the package, it is useful to just install
       a package previously built.

    --install: Installs apk to device/emulator. By default this step is
       not taken. If the application has previously been installed on
       the device, it will be uninstalled first.

    --reinstall: Installs apk to device/emulator. By default this step
       is not taken. If the application has previously been installed on
       the device, it will be overwritten, but its data will be left
       intact.

    --device [device ID]: Use specified device for deployment. Default
       is the device selected by default by adb.

    --android-platform <platform>: Builds against the given android
       platform. By default, the highest available version will be
       used.

    --release: Builds a package ready for release. By default, the
       package will be signed with a debug key.

    --sign <url/to/keystore> <alias>: Signs the package with the
       specified keystore, alias and store password.
       Optional arguments for use with signing:
         --storepass <password>: Keystore password.
         --storetype <type>: Keystore type.
         --keypass <password>: Password for private key (if different
           from keystore password.)
         --sigfile <file>: Name of .SF/.DSA file.
         --digestalg <name>: Name of digest algorithm. Default is
           "SHA1".
         --sigalg <name>: Name of signature algorithm. Default is
           "SHA1withRSA".
         --tsa <url>: Location of the Time Stamping Authority.
         --tsacert <alias>: Public key certificate for TSA.
         --internalsf: Include the .SF file inside the signature block.
         --sectionsonly: Don't compute hash of entire manifest.
         --protected: Keystore has protected authentication path.
         --jarsigner: Deprecated, ignored.

       NOTE: To conceal the keystore information, the environment variables
         QT_ANDROID_KEYSTORE_PATH, and QT_ANDROID_KEYSTORE_ALIAS are used to
         set the values keysotore and alias respectively.
         Also the environment variables QT_ANDROID_KEYSTORE_STORE_PASS,
         and QT_ANDROID_KEYSTORE_KEY_PASS are used to set the store and key
         passwords respectively. This option needs only the --sign parameter.

    --jdk <path/to/jdk>: Used to find the jarsigner tool when used
       in combination with the --release argument. By default,
       an attempt is made to detect the tool using the JAVA_HOME and
       PATH environment variables, in that order.

    --qml-import-paths: Specify additional search paths for QML
       imports.

    --verbose: Prints out information during processing.

    --no-generated-assets-cache: Do not pregenerate the entry list for
       the assets file engine.

    --aux-mode: Operate in auxiliary mode. This will only copy the
       dependencies into the build directory and update the XML templates.
       The project will not be built or installed.

    --apk <path/where/to/copy/the/apk>: Path where to copy the built apk.

    --qml-importscanner-binary <path/to/qmlimportscanner>: Override the
       default qmlimportscanner binary path. By default the
       qmlimportscanner binary is located using the Qt directory
       specified in the input file.

    --depfile <path/to/depfile>: Output a dependency file.

    --builddir <path/to/build/directory>: build directory. Necessary when
       generating a depfile because ninja requires relative paths.

    --no-rcc-bundle-cleanup: skip cleaning rcc bundle directory after
       running androiddeployqt. This option simplifies debugging of
       the resource bundle content, but it should not be used when deploying
       a project, since it litters the 'assets' directory.

    --copy-dependencies-only: resolve application dependencies and stop
       deploying process after all libraries and resources that the
       application depends on have been copied.

    --help: Displays this information.
)");
}

} // namespace deploy
} // namespace android