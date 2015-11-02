%define mainver 1.14i
%define optver 20050924p1
%define setupdir %{name}-%{mainver}-ac%{optver}

Name: lha
Version: %{mainver}.autoconf%{optver}
Release: 1
Source: %{name}-%{mainver}-ac%{optver}.tar.gz

Summary: LHa file archiving utility with compress
Summary(ja): LHa 高圧縮アーカイバ
License: distributable
Group: Applications/File
Url: http://lha.osdn.jp/
# Url: https://github.com/jca02266/lha
# Url: http://www2m.biglobe.ne.jp/~dolphin/lha/lha.htm
Packager: Yohei Endo <yoheie@gmail.com>

Buildroot: %{_tmppath}/%{name}-%{version}-root
Prefix: %{_prefix}

%description
LHa is a file archiving utility with compress.
This is autoconfiscated and bug-fixed version of original 1.14i.
Original 1.14i can be found at:
 http://www2m.biglobe.ne.jp/~dolphin/lha/prog/lha-114i.tar.gz

%description -l ja
LHa は効率の高い圧縮機能を持つファイルアーカイバです。
このバージョンはオリジナルの 1.14i に autoconfiscate および
いくつかの bug fix をしたものです。
オリジナルの 1.14i は以下の URI から入手可能です。
 http://www2m.biglobe.ne.jp/~dolphin/lha/prog/lha-114i.tar.gz

%prep
rm -rf ${RPM_BUILD_ROOT}
%setup -n %{setupdir}

%build
./configure --prefix=%{_prefix} --mandir=%{_mandir}/ja
make

%install
rm -rf ${RPM_BUILD_ROOT}
make prefix=${RPM_BUILD_ROOT}/%{_prefix} mandir=${RPM_BUILD_ROOT}/%{_mandir}/ja install

%clean
rm -rf ${RPM_BUILD_ROOT}

%files
%defattr(-,root,root)
%doc 00readme.autoconf header.doc.euc ChangeLog Hacking_of_LHa olddoc/
%{_prefix}/bin/lha
%{_mandir}/ja/mann/lha.n*

%changelog
* Mon Nov 02 2015 Yohei Endo <yoheie@gmail.com>
- Fix mandir on make install

* Tue Oct 13 2015 Yohei Endo <yoheie@gmail.com>
- Fix url for original 1.14i

* Mon Sep 28 2015 Yohei Endo <yoheie@gmail.com>
- Fix Urls

* Tue Sep 15 2015 Yohei Endo <yoheie@gmail.com>
- Remove old mail address
- Fix Url

* Tue May 10 2005 Yohei Endo
- Do not define name, version, release, source

* Sat Nov 01 2003 Yohei Endo
- Use %%{_prefix}/bin instead of %%{_bindir}

* Fri Feb 14 2003 Yohei Endo
- Summary(ja) changed

* Mon Feb 10 2003 Yohei Endo
- Write some useful definition

* Fri Feb 07 2003 Yohei Endo
- Chenged "Copyright" -> "License"
- Url changed
- description changed

* Tue Feb 04 2003 Yohei Endo
- Changed for Single archive

* Mon Oct 28 2002 Yohei Endo
- 1.14i.autoconf20020903-2
- Url and description are fixed

* Tue Sep 03 2002 Yohei Endo
- 1.14i.autoconf20020903-1
