#include "occonfiguration.h"
#include <QDebug>

OcConfiguration::OcConfiguration(QObject *parent) :
    QObject(parent)
{
    conf = new de::buschmann23::ocNewsEngine::Configuration("de.buschmann23.ocNewsEngine", "/Configuration", QDBusConnection::sessionBus(), this);

    QVariantMap config = conf->getConfig();
    m_privateBrowsing = config["privateBrowsing"].toBool();
    m_enableCookies = config["enableCookies"].toBool();
    m_maxItems = config["maxitems"].toInt();
    m_viewMode = config["viewmode"].toInt();
    m_orderBy = config["orderby"].toString();
    m_themeColor = config["themecolor"].toString();
    m_handleImgs = config["handleimgs"].toInt();
    m_handleRead = config["handleread"].toInt();
    m_sortAsc = config["sortasc"].toBool();
    m_fontSize = config["fontsize"].toInt();
    m_hideReadFeeds = config["hidereadfeeds"].toBool();
    m_showExcerpts = config["showExcerpts"].toBool();
    m_showPicturesInList = config["showPicturesInList"].toBool();
    m_updateBehavior = config["updatebehavior"].toInt();
    m_updateInterval = config["updateinterval"].toInt();
    m_eventFeeds = config["eventfeeds"].toString();
    m_quitEngine = config["quitengine"].toBool();
    m_notifyFeedsFolders = config["notifyFeedsFolders"].toBool();
    m_notifyNewItems = config["notifyNewItems"].toBool();
    m_isValid = conf->isConfigSet();
#if !defined(MEEGO_EDITION_HARMATTAN)
    m_accountEnabled = config["enabled"].toBool();
    m_accountUser = config["uname"].toString();
    m_accountPassword = config["pword"].toString();
    m_accountServer = config["server"].toString();
    m_accountUseSSL = config["usessl"].toBool();
    m_accountIgnoreSSLErrors = config["ignoresslerrors"].toBool();
    m_privacyShown = config["privacyShown"].toBool();
#endif
}


bool OcConfiguration::privateBrowsing() const { return m_privateBrowsing; }


void OcConfiguration::setPrivateBrowsing(const bool &nPrivateBrowsing)
{
    if (nPrivateBrowsing != m_privateBrowsing) {
        m_privateBrowsing = nPrivateBrowsing;
        conf->setSetting("privacy/privateBrowsing", QDBusVariant(privateBrowsing()));
        emit privateBrowsingChanged(privateBrowsing());
    }
}


bool OcConfiguration::enableCookies() const { return m_enableCookies; }

void OcConfiguration::setEnableCookies(const bool &nEnableCookies)
{
    if (nEnableCookies != m_enableCookies) {
        m_enableCookies = nEnableCookies;
        conf->setSetting("privacy/enableCookies", QDBusVariant(enableCookies()));
        emit enableCookiesChanged(enableCookies());
    }
}


int OcConfiguration::maxItems() const { return m_maxItems; }

void OcConfiguration::setMaxItems(const int &nMaxItems)
{
    if (nMaxItems != m_maxItems) {
        m_maxItems = nMaxItems;
        conf->setSetting("storage/maxitems", QDBusVariant(maxItems()));
        emit maxItemsChanged(maxItems());
    }
}


int OcConfiguration::viewMode() const { return m_viewMode; }

void OcConfiguration::setViewMode(const int &nViewMode)
{
    if (nViewMode != m_viewMode) {
        m_viewMode = nViewMode;
        conf->setSetting("display/viewmode", QDBusVariant(viewMode()));
        emit viewModeChanged(viewMode());
    }
}


QString OcConfiguration::orderBy() const { return m_orderBy; }

void OcConfiguration::setOrderBy(const QString &nOrderBy)
{
    if (nOrderBy != m_orderBy) {
        m_orderBy = nOrderBy;
        conf->setSetting("display/orderby", QDBusVariant(orderBy()));
        emit orderByChanged(orderBy());
    }
}


QString OcConfiguration::themeColor() const { return m_themeColor; }

void OcConfiguration::setThemeColor(const QString &nThemeColor)
{
    if (nThemeColor != m_themeColor) {
        m_themeColor = nThemeColor;
        conf->setSetting("display/themecolor", QDBusVariant(themeColor()));
        emit themeColorChanged(themeColor());
    }
}


int OcConfiguration::handleImgs() const { return m_handleImgs; }

void OcConfiguration::setHandleImgs(const int &nHandleImgs)
{
    if (nHandleImgs != m_handleImgs) {
        m_handleImgs = nHandleImgs;
        conf->setSetting("display/handleimgs", QDBusVariant(handleImgs()));
        emit handleImgsChanged(handleImgs());
    }
}


int OcConfiguration::handleRead() const { return m_handleRead; }

void OcConfiguration::setHandleRead(const int &nHandleRead)
{
    if (nHandleRead != m_handleRead) {
        m_handleRead = nHandleRead;
        conf->setSetting("display/handleread", QDBusVariant(handleRead()));
        emit handleReadChanged(handleRead());
    }
}


bool OcConfiguration::sortAsc() const { return m_sortAsc; }

void OcConfiguration::setSortAsc(const bool &nSortAsc)
{
    if (nSortAsc != m_sortAsc) {
        m_sortAsc = nSortAsc;
        conf->setSetting("display/sortasc", QDBusVariant(sortAsc()));
        emit sortAscChanged(sortAsc());
    }
}



int OcConfiguration::fontSize() const { return m_fontSize; }

void OcConfiguration::setFontSize(const int &nFontSIze)
{
    if (nFontSIze != m_fontSize) {
        m_fontSize = nFontSIze;
        conf->setSetting("display/fontsize", QDBusVariant(fontSize()));
        emit fontSizeChanged(fontSize());
    }
}


bool OcConfiguration::hideReadFeeds() const { return m_hideReadFeeds; }

void OcConfiguration::setHideReadFeeds(const bool &nHideReadFeeds)
{
    if (nHideReadFeeds != m_hideReadFeeds) {
        m_hideReadFeeds = nHideReadFeeds;
        conf->setSetting("display/hidereadfeeds", QDBusVariant(hideReadFeeds()));
        emit hideReadFeedsChanged(hideReadFeeds());
    }
}



bool OcConfiguration::showExcerpts() const { return m_showExcerpts; }

void OcConfiguration::setShowExcerpts(const bool &nShowExcerpts)
{
    if (nShowExcerpts != m_showExcerpts) {
        m_showExcerpts = nShowExcerpts;
        conf->setSetting("display/excerpts", QDBusVariant(showExcerpts()));
        emit showExcerptsChanged(showExcerpts());
    }
}


bool OcConfiguration::showPicturesInList() const { return m_showPicturesInList; }

void OcConfiguration::setShowPicturesInList(const bool &nShowPicturesInList)
{
    if (nShowPicturesInList != m_showPicturesInList) {
        m_showPicturesInList = nShowPicturesInList;
        conf->setSetting("display/picturesInList", QDBusVariant(showPicturesInList()));
        emit showPicturesInListChanged(showPicturesInList());
    }
}



int OcConfiguration::updateBehavior() const { return m_updateBehavior; }

void OcConfiguration::setUpdateBehavior(const int &nUpdateBehavior)
{
    if (nUpdateBehavior != m_updateBehavior) {
        m_updateBehavior = nUpdateBehavior;
        conf->setSetting("update/behavior", QDBusVariant(updateBehavior()));
        emit updateBehaviorChanged(updateBehavior());
    }
}


int OcConfiguration::updateInterval() const { return m_updateInterval; }

void OcConfiguration::setUpdateInterval(const int &nUpdateInterval)
{
    if (nUpdateInterval != m_updateInterval) {
        m_updateInterval = nUpdateInterval;
        conf->setSetting("update/interval", QDBusVariant(updateInterval()));
        emit updateIntervalChanged(updateInterval());
    }
}


QString OcConfiguration::eventFeeds() const { return m_eventFeeds; }

void OcConfiguration::setEventFeeds(const QString &nEventFeeds)
{
    if (nEventFeeds != m_eventFeeds) {
        m_eventFeeds = nEventFeeds;
        conf->setSetting("event/feeds", QDBusVariant(eventFeeds()));
        emit eventFeedsChanged(eventFeeds());
    }
}


bool OcConfiguration::quitEngine() const { return m_quitEngine; }

void OcConfiguration::setQuitEngine(const bool &nQuitEngine)
{
    if (nQuitEngine != m_quitEngine) {
        m_quitEngine = nQuitEngine;
        conf->setSetting("engine/quitonclose", QDBusVariant(quitEngine()));
        emit quitEngineChanged(quitEngine());
    }
}


bool OcConfiguration::notifyFeedsFolders() const { return m_notifyFeedsFolders; }

void OcConfiguration::setNotifyFeedsFolders(const bool &nNotifyFeedsFolders)
{
    if (nNotifyFeedsFolders != m_notifyFeedsFolders) {
        m_notifyFeedsFolders = nNotifyFeedsFolders;
        conf->setSetting("notifications/feedsFolders", QDBusVariant(notifyFeedsFolders()));
        emit notifyFeedsFoldersChanged(notifyFeedsFolders());
    }
}



bool OcConfiguration::notifyNewItems() const { return m_notifyNewItems; }

void OcConfiguration::setNotifyNewItems(const bool &nNotifyNewItems)
{
    if (nNotifyNewItems != m_notifyNewItems) {
        m_notifyNewItems = nNotifyNewItems;
        conf->setSetting("notifications/newItems", QDBusVariant(notifyNewItems()));
        emit notifyNewItemsChanged(notifyNewItems());
    }
}



QString OcConfiguration::textFormat() const { return m_textFormat; }

void OcConfiguration::setTextFormat(const QString &nTextFormat)
{
    if (nTextFormat != m_textFormat) {
        m_textFormat = nTextFormat;
        conf->setSetting("display/textformat", QDBusVariant(textFormat()));
        emit eventFeedsChanged(textFormat());
    }
}


bool OcConfiguration::privacyShown() const { return m_privacyShown; }

void OcConfiguration::setPrivacyShown(const bool &nPrivacyShown)
{
    if (nPrivacyShown != m_privacyShown) {
        m_privacyShown = nPrivacyShown;
        emit privacyShownChanged(privacyShown());
    }
}


bool OcConfiguration::isValid() const { return m_isValid; }

void OcConfiguration::setIsValid(const bool &nIsValid)
{
    if (nIsValid != m_isValid) {
        m_isValid = nIsValid;
        emit isValidChanged(isValid());
    }
}


#if !defined(MEEGO_EDITION_HARMATTAN)

bool OcConfiguration::accountEnabled() const { return m_accountEnabled; }

void OcConfiguration::setAccountEnabled(const bool &nAccountEnabled)
{
    if (nAccountEnabled != m_accountEnabled) {
        m_accountEnabled = nAccountEnabled;
        conf->setSetting("account/enabled", QDBusVariant(accountEnabled()));
        emit accountEnabledChanged(accountEnabled());
    }
}


QString OcConfiguration::accountUser() const { return m_accountUser; }

void OcConfiguration::setAccountUser(const QString &nAccountUser)
{
    if (nAccountUser != m_accountUser) {
        m_accountUser = nAccountUser;
        setIsValid(accountUser() != "" && accountPassword() !=  "" && accountServer() != "");
        conf->setSetting("account/user", QDBusVariant(accountUser()));
        emit accountUserChanged(accountUser());
    }
}


QString OcConfiguration::accountPassword() const { return m_accountPassword; }

void OcConfiguration::setAccountPassword(const QString &nAccountPassword)
{
    if (nAccountPassword != m_accountPassword) {
        m_accountPassword = nAccountPassword;
        setIsValid(accountUser() != "" && accountPassword() !=  "" && accountServer() != "");
        conf->setSetting("account/password", QDBusVariant(accountPassword()));
        emit accountPasswordChanged(accountPassword());
    }
}


QString OcConfiguration::accountServer() const { return m_accountServer; }

void OcConfiguration::setAccountServer(const QString &nAccountServer)
{
    if (nAccountServer != m_accountServer) {
        m_accountServer = nAccountServer;
        setIsValid(accountUser() != "" && accountPassword() !=  "" && accountServer() != "");
        conf->setSetting("account/server", QDBusVariant(accountServer()));
        emit accountServerChanged(accountServer());
    }
}


bool OcConfiguration::accountUseSSL() const { return m_accountUseSSL; }

void OcConfiguration::setAccountUseSSL(const bool &nAccountUseSSL)
{
    if (nAccountUseSSL != m_accountUseSSL) {
        m_accountUseSSL = nAccountUseSSL;
        conf->setSetting("account/usessl", QDBusVariant(accountUseSSL()));
        emit accountUseSSLChanged(accountUseSSL());
    }
}


bool OcConfiguration::accountIgnoreSSLErrors() const { return m_accountIgnoreSSLErrors; }

void OcConfiguration::setAccountIgnoreSSLErrors(const bool &nAccountIgnoreSSLErrors)
{
    if (nAccountIgnoreSSLErrors != m_accountIgnoreSSLErrors) {
        m_accountIgnoreSSLErrors = nAccountIgnoreSSLErrors;
        conf->setSetting("account/ignoresslerrors", QDBusVariant(accountIgnoreSSLErrors()));
        emit accountIgnoreSSLErrorsChanged(accountIgnoreSSLErrors());
    }
}

#endif
