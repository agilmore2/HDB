Name:           oracle-hdblib-compat
Version:        12.2.0.1
Release:        2
Summary:        Compatibility package so that perl-DBD-Oracle will install
Group:          Applications/Multimedia
License:        GPLv2
# Copied from Spacewalk's oracle-lib-compat at:
# This src.rpm is canonical upstream
# You can obtain it using this set of commands
# git clone git://git.fedorahosted.org/git/spacewalk.git/
# cd spec-tree/oracle-lib-compat
# make srpm
URL:            https://hdbsupport.precisionwre.com
#URL:            https://github.com/spacewalkproject/spacewalk
Source0:	https://github.com/spacewalkproject/spacewalk/archive/%{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-root-%(%{__id_u} -n)

%define debug_package %{nil}

%ifarch s390 s390x
%define icversion 10.2.0.4
%define icdir %{icversion}
Requires:       oracle-instantclient-basic = %{icversion}
Requires:       oracle-instantclient-sqlplus = %{icversion}
%define soversion 10
%else
%define icversion 12.2.0.1.0
%define icdir 12.2
%define orahome /ora1/app/product/%{icdir}/db
#Oracle not installed via rpm, can't require these when using full install
#Requires:       oracle-instantclient11.2-basic = %%{icversion}
#Requires:       oracle-instantclient11.2-sqlplus = %%{icversion}
%define soversion 12
%endif

Requires(post): ldconfig
Requires(post): /usr/bin/execstack
Requires(post): /usr/bin/file
Requires(post): /usr/bin/xargs

%ifarch x86_64
%define lib64 ()(64bit)
Requires:       libaio.so.1%{lib64}
%endif
Provides:       libocci.so.%{soversion}.1%{?lib64}   = %{icversion}
Provides:       libnnz%{soversion}.so%{?lib64}       = %{icversion}
Provides:       libocijdbc%{soversion}.so%{?lib64}   = %{icversion}
Provides:       libclntsh.so.%{soversion}.1%{?lib64} = %{icversion}
Provides:       libociei.so%{?lib64}                 = %{icversion}
Provides:       ojdbc14                              = %{icversion}
Requires:       libstdc++.so.6%{?lib64}

%description
HDB Compatibility package providing oracle libs from full install from Oracle installer.

%prep
%setup -q

%build

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT

mkdir -p $RPM_BUILD_ROOT%{_sysconfdir}/ld.so.conf.d
echo %{orahome}/lib >>$RPM_BUILD_ROOT%{_sysconfdir}/ld.so.conf.d/oracle-%{icdir}.conf

mkdir -p $RPM_BUILD_ROOT%{_bindir}
ln -s %{orahome}/bin/sqlplus $RPM_BUILD_ROOT%{_bindir}/sqlplus

# Spacewalk turned this off to avoid directory creation in /root
# HDB probably doesn't care, and shouldn't overwrite this file in case something is set
#mkdir -p $RPM_BUILD_ROOT%{orahome}/network/admin
#echo 'diag_adr_enabled = off' > $RPM_BUILD_ROOT%{orahome}/network/admin/sqlnet.ora

mkdir -p $RPM_BUILD_ROOT/%{_javadir}
ln -s %{orahome}/jdbc/lib/ojdbc6.jar $RPM_BUILD_ROOT/%{_javadir}

%clean
rm -rf $RPM_BUILD_ROOT

%files
%{_bindir}/sqlplus
#/usr/lib/oracle/11.2/client/lib/network/admin/sqlnet.ora
%config(noreplace) %{_sysconfdir}/ld.so.conf.d/oracle-%{icdir}.conf
%{_javadir}/ojdbc6.jar

%post
ldconfig

# clear execstack on libs in oracle's library directory
find %{orahome}/lib \
        | xargs file | awk -F: '/ELF.*(executable|shared object)/ {print $1}' \
        | xargs execstack -c

%changelog
* Fri Dec 22 2017 Andrew Gilmore <andrew@precisionwre.com> 12.2.0.1-1
- adapt for oracle 12cR2

* Mon Jul 17 2017 Jan Dobes 11.2.0.14-1
- Remove more fedorahosted links
- Updated links to github in spec files
- Migrating Fedorahosted to GitHub

* Sat Sep 26 2015 Andrew Gilmore <andrew@precisionwre.com> 11.2.0.13-1
- adapt for full oracle install
- assume 64bit (yay!)
- remove setting in sqlnet.ora for diag_adr_enabled

* Thu Jan 29 2015 Tomas Lestach <tlestach@redhat.com> 11.2.0.12-1
- we need to use the exact oracle instantclient version

* Thu Jan 29 2015 Tomas Lestach <tlestach@redhat.com> 11.2.0.11-1
- do not require exact version of oracle instantclient

* Wed Oct 22 2014 Michael Mraka <michael.mraka@redhat.com> 11.2.0.10-1
- oracle-instantclient11.2 requires libstdc++.so.6

* Wed Jan 22 2014 Michael Mraka <michael.mraka@redhat.com> 11.2.0.9-1
- LD_PRELOAD setup has been moved to spacewalk-setup-tomcat
- Purging %%changelog entries preceding Spacewalk 1.0, in active packages.

* Tue Dec 04 2012 Jan Pazdziora 11.2.0.8-1
- On Fedoras, start to use tomcat >= 7.

* Mon Oct 08 2012 Jan Pazdziora 11.2.0.7-1
- Require latest greatest oracle-instantclient11.2-*.
- %%defattr is not needed since rpm 4.4

* Fri Feb 03 2012 Jan Pazdziora 11.2.0.6-1
- Avoid cat: write error: Broken pipe when calling tomcat service under trap ''
  PIPE

* Mon May 16 2011 Jan Pazdziora 11.2.0.5-1
- Both tomcat5 and tomcat6 which needs the LD_PRELOAD set.

* Wed May 04 2011 Jan Pazdziora 11.2.0.4-1
- We unset LD_PRELOAD to force ldd to show the libldap line with => even if
  LD_PRELOAD was already set.

* Mon Jan 17 2011 Jan Pazdziora 11.2.0.3-1
- Set diag_adr_enabled to off.

* Mon Jan 10 2011 Jan Pazdziora 11.2.0.2-1
- On x86_64, require 64bit version of libaio for InstantClient 11g.

* Fri Jan 07 2011 Jan Pazdziora 11.2.0.1-1
- Have separate ld.so.conf.d for InstantClient and for XE server.
- InstantClient 11 contains ojdbc5 and ojdbc6, we will change the target of the
  symlink for now.
- Need to use the "11" in .so Provides as well.
- Switch to Oracle InstantClient 11 in oracle-lib-compat.

* Thu Sep 23 2010 Michael Mraka <michael.mraka@redhat.com> 10.2.0.25-1
- instantclient on s390(x) upgraded to 10.2.0.4
- switched to default VersionTagger

* Thu Sep 23 2010 Jan Pazdziora 10.2-24
- 623115 - file lookup using just the linker name (libldap.so) fails if
  openldap-devel is not installed.

* Mon Sep 13 2010 Jan Pazdziora 10.2-23
- 623115 - force tomcat to use the stock openldap, overriding the ldap_*
  symbols in libclntsh.so*.

