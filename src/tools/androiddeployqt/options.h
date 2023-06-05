// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QTBASE_TOOLS_ANDROIDDEPLOYQT_OPTIONS_H
#define QTBASE_TOOLS_ANDROIDDEPLOYQT_OPTIONS_H

#include <QString>
#include <QStringList>

using namespace Qt::StringLiterals;

struct QtInstallDirectoryWithTriple
{
    QtInstallDirectoryWithTriple(const QString &dir = QString(),
                                 const QString &t = QString(),
                                 const QHash<QString, QString> &dirs = QHash<QString, QString>()
    ) :
            qtInstallDirectory(dir),
            qtDirectories(dirs),
            triple(t),
            enabled(false)
    {}

    QString qtInstallDirectory;
    QHash<QString, QString> qtDirectories;
    QString triple;
    bool enabled;
};

struct QtDependency
{
    QtDependency(const QString &rpath, const QString &apath) : relativePath(rpath), absolutePath(apath) {}

    bool operator==(const QtDependency &other) const
    {
        return relativePath == other.relativePath && absolutePath == other.absolutePath;
    }

    QString relativePath;
    QString absolutePath;
};

struct Options
{
    Options()
            : helpRequested(false)
            , verbose(false)
            , timing(false)
            , build(true)
            , auxMode(false)
            , deploymentMechanism(Bundled)
            , releasePackage(false)
            , digestAlg("SHA-256"_L1)
            , sigAlg("SHA256withRSA"_L1)
            , internalSf(false)
            , sectionsOnly(false)
            , protectedAuthenticationPath(false)
            , installApk(false)
            , uninstallApk(false)
            , qmlImportScannerBinaryPath()
    {}

    enum DeploymentMechanism
    {
        Bundled,
        Unbundled
    };

    enum TriState {
        Auto,
        False,
        True
    };

    bool helpRequested;
    bool verbose;
    bool timing;
    bool build;
    bool auxMode;
    bool noRccBundleCleanup = false;
    bool copyDependenciesOnly = false;

    // External tools
    QString sdkPath;
    QString sdkBuildToolsVersion;
    QString ndkPath;
    QString ndkVersion;
    QString jdkPath;

    // Build paths
    QString qtInstallDirectory;
    QHash<QString, QString> qtDirectories;
    QString qtDataDirectory;
    QString qtLibsDirectory;
    QString qtLibExecsDirectory;
    QString qtPluginsDirectory;
    QString qtQmlDirectory;
    QString qtHostDirectory;
    std::vector<QString> extraPrefixDirs;
    // Unlike 'extraPrefixDirs', the 'extraLibraryDirs' key doesn't expect the 'lib' subfolder
    // when looking for dependencies.
    std::vector<QString> extraLibraryDirs;
    QString androidSourceDirectory;
    QString outputDirectory;
    QString inputFileName;
    QString applicationBinary;
    QString applicationArguments;
    std::vector<QString> rootPaths;
    QString rccBinaryPath;
    QString depFilePath;
    QString buildDirectory;
    QStringList qmlImportPaths;
    QStringList qrcFiles;

    // Versioning
    QString versionName;
    QString versionCode;
    QByteArray minSdkVersion{"23"};
    QByteArray targetSdkVersion{"31"};

    // lib c++ path
    QString stdCppPath;
    QString stdCppName = QStringLiteral("c++_shared");

    // Build information
    QString androidPlatform;
    QHash<QString, QtInstallDirectoryWithTriple> architectures;
    QString currentArchitecture;
    QString toolchainPrefix;
    QString ndkHost;
    bool buildAAB = false;
    bool isZstdCompressionEnabled = false;


    // Package information
    DeploymentMechanism deploymentMechanism;
    QString systemLibsPath;
    QString packageName;
    QStringList extraLibs;
    QHash<QString, QStringList> archExtraLibs;
    QStringList extraPlugins;
    QHash<QString, QStringList> archExtraPlugins;

    // Signing information
    bool releasePackage;
    QString keyStore;
    QString keyStorePassword;
    QString keyStoreAlias;
    QString storeType;
    QString keyPass;
    QString sigFile;
    QString signedJar;
    QString digestAlg;
    QString sigAlg;
    QString tsaUrl;
    QString tsaCert;
    bool internalSf;
    bool sectionsOnly;
    bool protectedAuthenticationPath;
    QString apkPath;

    // Installation information
    bool installApk;
    bool uninstallApk;
    QString installLocation;

    // Per architecture collected information
    void setCurrentQtArchitecture(const QString &arch,
                                  const QString &directory,
                                  const QHash<QString, QString> &directories)
    {
        currentArchitecture = arch;
        qtInstallDirectory = directory;
        qtDataDirectory = directories["qtDataDirectory"_L1];
        qtLibsDirectory = directories["qtLibsDirectory"_L1];
        qtLibExecsDirectory = directories["qtLibExecsDirectory"_L1];
        qtPluginsDirectory = directories["qtPluginsDirectory"_L1];
        qtQmlDirectory = directories["qtQmlDirectory"_L1];
    }
    typedef QPair<QString, QString> BundledFile;
    QHash<QString, QList<BundledFile>> bundledFiles;
    QHash<QString, QList<QtDependency>> qtDependencies;
    QHash<QString, QStringList> localLibs;
    bool usesOpenGL = false;

    // Per package collected information
    QStringList initClasses;
    QStringList permissions;
    QStringList features;

    // Override qml import scanner path
    QString qmlImportScannerBinaryPath;
    bool qmlSkipImportScanning = false;
};


#endif // QTBASE_TOOLS_ANDROIDDEPLOYQT_OPTIONS_H
