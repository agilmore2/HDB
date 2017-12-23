Summary: SysV init and sysconfig for Oracle11g
Name: oraclerun
Version: 12cR2.1.5
Release: 1
License: Public Domain
Packager: andrew@precisionwre.com
Source: %{name}-%{version}.tar.gz
Group: Applications/Databases
Buildroot: %{_tmppath}/%{name}-root
BuildArch: noarch

#From Oracle Linux repositories, resigned with HDB key to avoid multiplying repos
Requires: oracle-database-server-12cR2-preinstall

%description
Files that integrate Oracle12c into the normal config files for
Fedora Core, Red Hat Enterprise Linux, CentOS and perhaps SUSE SLES.  This
includes a SysV init script (not systemd yet), a sysconfig file,
and the environment configuration files for /etc/profile.d.

%prep
rm -rf $RPM_BUILD_ROOT
%setup -q

%build

%install

mkdir -p $RPM_BUILD_ROOT/usr/share/doc/%{name}-%{version}
mkdir -p $RPM_BUILD_ROOT/etc/init.d
mkdir $RPM_BUILD_ROOT/etc/sysconfig
mkdir $RPM_BUILD_ROOT/etc/profile.d

cp -p $RPM_BUILD_DIR/%{name}-%{version}/etc/init.d/oracle $RPM_BUILD_ROOT/etc/init.d
cp -p $RPM_BUILD_DIR/%{name}-%{version}/etc/profile.d/oracle* $RPM_BUILD_ROOT/etc/profile.d
cp -p $RPM_BUILD_DIR/%{name}-%{version}/etc/sysconfig/oracle $RPM_BUILD_ROOT/etc/sysconfig
cp -p $RPM_BUILD_DIR/%{name}-%{version}/usr/share/doc/%{name}-%{version}/* $RPM_BUILD_ROOT/usr/share/doc/%{name}-%{version}

%post
chkconfig --add oracle

%postun
if [ $1 -eq 0 ] ; then #if we're uninstalling, not upgrading
  chkconfig --del oracle
fi

%clean
rm -rf $RPM_BUILD_ROOT $RPM_BUILD_DIR/%{name}-%{version}/

%files
%defattr(-,root,root)
/etc/init.d/oracle
/etc/profile.d/oracle*
/etc/sysconfig/oracle
%doc /usr/share/doc/%{name}-%{version}/*

%changelog
* Fri Dec 22 2017 Andrew Gilmore <andrew@precisionwre.com> - 1.5-1
- Updated to Oracle 12c

* Fri Sep 11 2015 Andrew Gilmore <andrew@precisionwre.com> - 1.4-2
- Update oracle init script to start listener. Why was it removed?
- Add %post chkconfig --add oracle

* Wed Sep 9 2015 Andrew Gilmore <andrew@precisionwre.com> - 1.4-1
- Update name of NLS environment variable to ORA_NLS11

* Thu Jul 16 2015 Andrew Gilmore <andrew@precisionwre.com> - 1.3-4
- where did -3 go?
- Updated packager, references CentOS and 11g now

* Wed Nov 14 2012 Andrew Gilmore <agilmore@usbr.gov> - 1.3-2
- altered requires Oracle rpm name to oracle-db-preinstall

* Wed Nov 14 2012 Andrew Gilmore <agilmore@usbr.gov> - 1.3-1
- Changed name to oracle11g
- Updated to handle RHEL6 and clones
- System supplied JDK assumed sufficient, removed specific path set for JVM
- Updated to install alongside Oracle provided oracle-rdbms-server-11gR2-preinstall rpm
  - Removed kernel sysctl settings

* Mon Jan 28 2008 Andrew Gilmore <agilmore@uc.usbr.gov> - 1.2-1
- Updated to handle RHEL5 requirements, follows Oracle install advice at
- https://metalink.oracle.com document id 421308.1
- Also adds provision to start ISQLPLUS if desired,
- and a first attempt at starting the HTTP Server portion of
- Oracle Application Server

* Thu Apr 13 2006 Andrew Gilmore <agilmore@uc.usbr.gov> - 1.1-1
- Deleted redhat-release requirement, since SUSE can also 
- use this script, I hope.

* Wed Nov 2 2005 Andrew Gilmore <agilmore@uc.usbr.gov>
- Ver 1.0
- Adapted from the excellent oraclerun9i from 
- Dax Kelson <dax@gurulabs.com>, Version 1.4
- see http://www.gurulabs.com/goodies/downloads.php
- Probably cut too many things from the init script, 
- but who runs isqlplus and ultrasearch anyway? (Beside you, Mark!)
