%define name    minishoot
%define version 1.0
%define release 1
%define buildroot %{_tmppath}/%{name}-%{version}-root
%define _prefix /usr/local

Summary: this is a sample game for practice ncurses
Name: %{name} 
Version: %{version}
Release: %{release} 
URL: http://taichino.com
Source: %{name}-%{version}.tar.gz
License: MIT
Group: game
BuildRoot: %{buildroot}

%description
This is sample game for practice ncurses

%prep
echo "prep phase"
%setup -q

%build
echo "build phase"
./configure 
make

%install 
echo "install phase"
rm -rf $RPM_BUILD_ROOT
%makeinstall

%clean
echo "clean phase"
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/local/games/minishoot
