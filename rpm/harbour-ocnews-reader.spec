# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.27
# 

Name:       harbour-ocnews-reader

# >> macros
%define __requires_exclude libQt5Declarative.*|libQt5Script.*|libQt5Widgets.*
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    ownCloud News App Client
Version:    1.7.1
Release:    1
Group:      Qt/Qt
License:    GPL-2.0
URL:        http://example.org/
Source0:    %{name}-%{version}.tar.bz2
Source100:  harbour-ocnews-reader.yaml
Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   qt5-plugin-imageformat-gif
Requires:   qt5-plugin-imageformat-ico
BuildRequires:  pkgconfig(Qt5Sql)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(sailfishapp) >= 0.0.10
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils

%description
ocNews is a client for the ownCloud News app that supports the complete API and all functions of the news app. It consists of a reader that communicates with a background daemon through DBus.


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5 

%qtc_make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%defattr(644, root, root, 755)
%attr(755, root, root) %{_bindir}/harbour-ocnews-*
%{_datadir}/dbus-1/interfaces/de.buschmann23.ocNewsEngine.*.xml
%{_datadir}/dbus-1/services/de.buschmann23.*.service
%{_datadir}/harbour-ocnews-reader/
%{_datadir}/applications/harbour-ocnews-reader.desktop
%{_datadir}/icons/hicolor/86x86/apps/harbour-ocnews-reader.png
# >> files
# << files
