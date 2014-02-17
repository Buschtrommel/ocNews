TEMPLATE = subdirs

VERSION = 1.7.0

!isEmpty(MEEGO_VERSION_MAJOR) {
OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_harmattan/postinst
} else {
OTHER_FILES += \
    rpm/harbour-ocnews-reader.spec \
    rpm/harbour-ocnews-reader.yaml
}

!isEmpty(MEEGO_VERSION_MAJOR) {
SUBDIRS += \
    reader \
    engine \
    account
} else {
SUBDIRS += \
    reader \
    engine
}
