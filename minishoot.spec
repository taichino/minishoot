%define name    minishoot
%define version 1.0
%define release 1
%define prefix /usr/local/games

Summary: this is a sample game for practice ncurses
Name: %{name} 
Version: %{version}
Release: %{release} 
URL: http://taichino.com
Source: minishoot.tar.gz
License: MIT
Group: game

%description
This is sample game for practice ncurses

%prep
echo "prep phase"
%setup -n minishoot

%build
echo "build phase"
make -e prefix=${RPM_BUILD_ROOT}%{prefix} install

%install 
echo "install phase"
%makeinstall

%clean
echo "clean phase"
make clean

%files
%{prefix}/minishoot
