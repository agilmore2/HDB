%define name oracle-database-server-12cR2-preinstall 
%define version 1.0
%define release 3hdb.el7
%global debug_package %{nil}

Summary: Sets the system for Oracle Database single instance and Real Application Cluster install for Oracle Linux 7
Name: %{name}
Version: %{version}
Release: %{release}
Group: Test Environment/Libraries
License: GPLv2
Vendor:Oracle
Source: %{name}-%{version}.tar.gz

Provides: %{name} = %{version}

Requires(pre):/etc/redhat-release

#System requirement
Requires:procps module-init-tools ethtool initscripts
Requires:bind-utils nfs-utils util-linux-ng pam
Requires:xorg-x11-utils xorg-x11-xauth 
#Requires:kernel-uek #Oracle kernel games not required
Requires:smartmontools


#As per docs
Requires:binutils compat-libstdc++-33 glibc glibc-devel 
Requires:ksh libaio libaio-devel libgcc libstdc++ libstdc++-devel 
Requires:make sysstat openssh-clients compat-libcap1 

#As per Orabug 20063241, 22250148, 22829306
Requires:psmisc net-tools unzip

BuildRoot: %{_builddir}/%{name}-%{version}-root

%description
The Oracle Preinstallation RPM package installs software packages and sets system parameters required for Oracle Database single instance and Oracle Real Application Clusters installations for Oracle Linux Release 7
Files affected: /etc/sysctl.conf, /boot/grub/menu.lst OR /boot/grub2/grub.cfg 
Files added: /etc/security/limits.d/oracle-database-server-12cR2-preinstall.conf


%prep
echo RPM_BUILD_ROOT=$RPM_BUILD_ROOT
%setup -q


%build

%install
rm -rf $RPM_BUILD_ROOT

mkdir -p -m 755 $RPM_BUILD_ROOT/etc/sysconfig/%{name}
mkdir -p -m 755 $RPM_BUILD_ROOT/usr/bin
mkdir -p -m 755 $RPM_BUILD_ROOT/etc/rc.d/init.d
mkdir -p -m 755 $RPM_BUILD_ROOT/etc/security/limits.d
mkdir -p -m 700 $RPM_BUILD_ROOT/var/log/%{name}/results

install -m 700 oracle-database-server-12cR2-preinstall-verify $RPM_BUILD_ROOT/etc/sysconfig/%{name}
install -m 700 oracle-database-server-12cR2-preinstall-verify $RPM_BUILD_ROOT/usr/bin
install -m 600 oracle-database-server-12cR2-preinstall.param $RPM_BUILD_ROOT/etc/sysconfig/%{name}
install -m 700 oracle-database-server-12cR2-preinstall-firstboot $RPM_BUILD_ROOT/etc/rc.d/init.d
touch $RPM_BUILD_ROOT/etc/security/limits.d/oracle-database-server-12cR2-preinstall.conf


ln -f -s /etc/sysconfig/%{name}/oracle-database-server-12cR2-preinstall-verify $RPM_BUILD_ROOT/usr/bin/oracle-database-server-12cR2-preinstall-verify 

%clean
rm -rf $RPM_BUILD_ROOT

%pre

if [ -f  /etc/sysconfig/%{name}/oracle-database-server-12cR2-preinstall.param ]; then 
  cp -f /etc/sysconfig/%{name}/oracle-database-server-12cR2-preinstall.param /var/log/%{name}/results/.oracle-database-server-12cR2-preinstall.param 
fi

if [ -d /etc/sysconfig/%{name} ]; then
  rm -rf /etc/sysconfig/%{name} 
fi	

%preun
if [ "$1" = "0" ] ; then # last uninstall
 chkconfig --del oracle-database-server-12cR2-preinstall-firstboot
 if [ -x /usr/bin/oracle-database-server-12cR2-preinstall-verify ]; then
   /usr/bin/oracle-database-server-12cR2-preinstall-verify -u 2> /dev/null 1>&2
 fi
fi

%posttrans
/usr/bin/oracle-database-server-12cR2-preinstall-verify 2>/dev/null 1>&2
if ! [ -f /etc/sysconfig/%{name}/oracle-database-server-12cR2-preinstall.conf ]; then
  chkconfig --add oracle-database-server-12cR2-preinstall-firstboot
fi

%postun
if [ "$1" = "0" ] ; then # last uninstall
 if [ -d /etc/sysconfig/%{name} ]; then
   rm -rf /etc/sysconfig/%{name} 
 fi
 if [ -d /var/log/%{name} ]; then	
   rm -rf /var/log/%{name} 	
 fi
fi

%files
%defattr(-,root,root)
%config /etc/sysconfig/%{name}/oracle-database-server-12cR2-preinstall.param
/etc/sysconfig/%{name}/oracle-database-server-12cR2-preinstall-verify
/etc/sysconfig/%{name}
%ghost /etc/security/limits.d/oracle-database-server-12cR2-preinstall.conf
/etc/rc.d/init.d/oracle-database-server-12cR2-preinstall-firstboot
/var/log/%{name}
/usr/bin/oracle-database-server-12cR2-preinstall-verify

%changelog
* Fri Dec 22 2017 Andrew Gilmore <andrew@precisionwre.com> [1.0-3hdb.el7]
- removed dependency on Oracle Linux specific kernel
- .spec file copied from -2, where are -3 sources?

* Fri Jun 09 2017 Vasundhara V <vasundhara.venkatasubramanian@oracle.com> [1.0-3.el7]
- Added new user/groups [Orabug 25045020]
- Fix upgrade issues [Orabug 26244431]
- Added oraclelinux-release to dependencies [Orabug 26244588]

* Tue Oct 18 2016 Vasundhara V <vasundhara.venkatasubramanian@oracle.com> [1.0-2.el7]
- Fixed Orabugs 24828130 24903029 24903070

* Tue Aug 17 2016  Vasundhara V <vasundhara.venkatasubramanian@oracle.com> [1.0-1.el7]
 - Initial version with new name
