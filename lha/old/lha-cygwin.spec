%define name lha
%define mainver 1.14i
%define optver 20030324
%define setupdir %{name}-%{mainver}-ac%{optver}

%define version %{mainver}.autoconf%{optver}
%define release 1
%define source %{name}-%{mainver}-ac%{optver}.tar.gz

Name: %{name}
Version: %{version}
Release: %{release}
Source: %{source}

Summary: LHa file archiving utility with compress
Summary(ja): LHa 高圧縮アーカイバ
License: distributable
Group: Applications/File
Url: http:/sourceforge.jp/projects/lha
# Url: http://www2m.biglobe.ne.jp/~dolphin/lha/lha.htm
# Url: http://ns103.net/~arai/lha/
Packager: Yohei ENDO <yoheie@ap3.mopera.ne.jp>

Buildroot: %{_tmppath}/%{name}-%{version}-root
Prefix: %{_prefix}

%description
LHa is a file archiving utility with compress.
This is autoconfiscated and bug-fixed version of original 1.14i.
Original 1.14i can be found at:
 http://www2m.biglobe.ne.jp/~dolphin/lha/prog/lha-1.14i.tar.gz
Patch for 1.14i can be found at:
 http://ns103.net/~arai/lha/

%description -l ja
LHa は効率の高い圧縮機能を持つファイルアーカイバです。
このバージョンはオリジナルの 1.14i に autoconfiscate および
いくつかの bug fix をしたものです。
オリジナルの 1.14i は以下の URI から入手可能です。
 http://www2m.biglobe.ne.jp/~dolphin/lha/prog/lha-1.14i.tar.gz
1.14i に対するパッチは以下の URI から入手可能です。
 http://ns103.net/~arai/lha/

%prep
rm -rf ${RPM_BUILD_ROOT}
%setup -n %{setupdir}

%build
./configure --prefix=%{_prefix} --mandir=%{_mandir}/ja
make

%install
rm -rf ${RPM_BUILD_ROOT}
make prefix=${RPM_BUILD_ROOT}/%{_prefix} mandir=${RPM_BUILD_ROOT}%{_mandir}/ja install

%clean
rm -rf ${RPM_BUILD_ROOT}

%files
%defattr(-,root,root)
%doc 00readme.autoconf header.doc.euc ChangeLog Hackinf_of_LHa olddoc/
%{_bindir}/lha.exe
%{_mandir}/ja/mann/lha.n*

%changelog
* Fri Feb 14 2003 Yohei ENDO <yoheie@ap3.mopera.ne.jp>
- Summary(ja) changed

* Mon Feb 10 2003 Yohei ENDO <yoheie@ap3.mopera.ne.jp>
- Write some useful definition

* Fri Feb 07 2003 Yohei ENDO <yoheie@ap3.mopera.ne.jp>
- Chenged "Copyright" -> "License"
- Url changed
- description changed

* Tue Feb 04 2003 Yohei ENDO <yoheie@ap3.mopera.ne.jp>
- Changed for Single archive

* Mon Oct 28 2002 Yohei ENDO <yoheie@ap3.mopera.ne.jp>
- 1.14i.autoconf20020903-2
- Url and description are fixed

* Tue Sep 03 2002 Yohei ENDO <yoheie@ap3.mopera.ne.jp>
- 1.14i.autoconf20020903-1
