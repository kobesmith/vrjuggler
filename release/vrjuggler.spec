# Spec file for vrjuggler.
%define name vrjuggler
%define global_version 2.0.2
%define global_release 1

%define vpr_version 1.0.2
%define vpr_release %{global_release}%{?dist}
%define tweek_version 1.0.2
%define tweek_release %{global_release}%{?dist}
%define jccl_version 1.0.2
%define jccl_release %{global_release}%{?dist}
%define sonix_version 1.0.2
%define sonix_release %{global_release}%{?dist}
%define gadgeteer_version 1.0.2
%define gadgeteer_release %{global_release}%{?dist}
%define vrjuggler_version %{global_version}
%define vrjuggler_release %{global_release}%{?dist}

# JDK 1.5.0 is the lowest version with native support for the x86_64
# architecture.
%ifarch x86_64
%define min_jdk 1.5.0
%else
%define min_jdk 1.4.2
%endif

%ifarch i386 i486 i586
%define vj_arch i686
%else
%define vj_arch %{_arch}
%endif

%define have_omniorb %(if [ -x /usr/bin/omniidl ] ; then echo 1; else echo 0; fi)
%define have_java %(if [ -x $JDK_HOME/bin/javac ] ; then echo 1; else echo 0; fi)
%define have_audiere %(if [ -x /usr/bin/audiere-config ] ; then echo 1; else echo 0; fi)
%define have_openal %(if [ -x /usr/bin/openal-config ] ; then echo 1; else echo 0; fi)
%define have_performer %(if [ -e /usr/lib/libpf.so ] ; then echo 1; else echo 0; fi)

Name: vrjuggler
Summary: VR Juggler is the virtual platform for VR application development
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
Source: %{name}-%{global_version}-%{global_release}.src.tar.bz2
URL: http://www.vrjuggler.org/
Group: Development/Libraries
BuildRoot: %{_tmppath}/%{name}-%{global_version}-%{global_release}-buildroot
License: LGPL
Requires: vpr = %{vpr_version}-%{vpr_release}
Requires: jccl-c++ = %{jccl_version}-%{jccl_release}
Requires: sonix = %{sonix_version}-%{sonix_release}
Requires: gadgeteer = %{gadgeteer_version}-%{gadgeteer_release}
Provides: vrjuggler = %{vrjuggler_version}-%{vrjuggler_release}

%description
VR Juggler provides virtual reality (VR) software developers with a suite of
application programming interfaces (APIs) that abstract, and hence simplify,
all interface aspects of their program including the display surfaces, object
tracking, selection and navigation, graphics rendering engines, and graphical
user interfaces. An application written with VR Juggler is essentially
independent of device, computer platform, and VR system. VR Juggler may be run
with any combination of immersive technologies and computational hardware.

%package -n vpr
Summary: The VR Juggler Portable Runtime (VPR)
Version: %{vpr_version}
Release: %{vpr_release}
URL: http://www.vrjuggler.org/vapor/
Group: Development/C++
Requires: cppdom >= 0.3.3
Requires: boost >= 1.32
BuildRequires: glibc-devel
BuildRequires: e2fsprogs-devel
BuildRequires: cppdom-devel >= 0.3.3
BuildRequires: boost-devel >= 1.32
Provides: vpr = %{vpr_version}-%{vpr_release}

%description -n vpr
The VR Juggler Portable Runtime (VPR) provides an object-oriented
operating system abstraction layer that allows for easy development of
cross-platform C++ applications.

%package -n vpr-devel
Summary: The VR Juggler Portable Runtime Headers
Version: %{vpr_version}
Release: %{vpr_release}
URL: http://www.vrjuggler.org/vapor/
Group: Development/C++
Requires: cppdom-devel >= 0.3.3
Requires: boost-devel >= 1.32
Requires: vpr = %{vpr_version}-%{vpr_release}
Provides: vpr-devel = %{vpr_version}-%{vpr_release}

%description -n vpr-devel
The header files and libraries needed for developing programs using the
VR Juggler Portable Runtime.

%package -n tweek-c++
Summary: The Tweek C++ API
Version: %{tweek_version}
Release: %{tweek_release}
URL: http://www.vrjuggler.org/tweek/
Group: Development/C++
Requires: libomniorb >= 4.0.6-2
Requires: vpr = %{vpr_version}-%{vpr_release}
BuildRequires: libomniorb-devel >= 4.0.6-2
Provides: tweek-c++ = %{tweek_version}-%{tweek_release}

%description -n tweek-c++
The Tweek C++ API provides a wrapper around CORBA for writing applications
based on the distributed model/view/controller system. In the scope of
VR Juggler, such applications are usually immersive worlds that have a
GUI that communicates with the immersive application through a network
connection.

%package -n tweek-c++-devel
Summary: The Tweek C++ API Headers
Version: %{tweek_version}
Release: %{tweek_release}
URL: http://www.vrjuggler.org/tweek/
Group: Development/C++
Requires: libomniorb-devel >= 4.0.6-2
Requires: vpr-devel = %{vpr_version}-%{vpr_release}
Requires: tweek-c++ = %{tweek_version}-%{tweek_release}
Provides: tweek-c++-devel = %{tweek_version}-%{tweek_release}

%description -n tweek-c++-devel
The header files and libraries needed for developing programs using the
Tweek C++ API.

%package -n tweek-java
Summary: The Tweek Java API and GUI
Version: %{tweek_version}
Release: %{tweek_release}
URL: http://www.vrjuggler.org/tweek/
Group: Development/Java
Requires: j2sdk >= %{min_jdk}
Provides: tweek-java = %{tweek_version}-%{tweek_release}
AutoReqProv: no

%description -n tweek-java
The Tweek Java API provides a wrapper around CORBA for writing applications
based on the distributed model/view/controller system. In the scope of
VR Juggler, such applications are usually immersive worlds that have a
GUI that communicates with the immersive application through a network
connection. The Tweek Java GUI provides an framework for creating such
GUIs. Application programmers write JavaBeans that plug into the Tweek
Java GUI.

%package -n jccl-c++
Summary: The JCCL C++ API
Version: %{jccl_version}
Release: %{jccl_release}
URL: http://www.vrjuggler.org/jccl/
Group: Development/C++
Requires: vpr = %{vpr_version}-%{vpr_release}
Provides: jccl-c++ = %{jccl_version}-%{jccl_release}

%description -n jccl-c++
The JCCL C++ API provides a dynamic configuration system based on XML.

%package -n jccl-c++-devel
Summary: The JCCL C++ API Headers
Version: %{jccl_version}
Release: %{jccl_release}
URL: http://www.vrjuggler.org/jccl/
Group: Development/C++
Requires: vpr-devel = %{vpr_version}-%{vpr_release}
Provides: jccl-c++-devel = %{jccl_version}-%{jccl_release}

%description -n jccl-c++-devel
The header files and libraries needed for developing programs using the
JCCL C++ API.

%package -n jccl-rtrc-plugin-c++
Summary: The JCCL Remote Run-Time Reconfiguration C++ Plug-in
Version: %{jccl_version}
Release: %{jccl_release}
URL: http://www.vrjuggler.org/jccl/
Group: Development/C++
Requires: vpr = %{vpr_version}-%{vpr_release}
Requires: tweek-c++ = %{tweek_version}-%{tweek_release}
Requires: jccl-c++ = %{jccl_version}-%{jccl_release}
Provides: jccl-rtrc-plugin-c++ = %{jccl_version}-%{jccl_release}

%description -n jccl-rtrc-plugin-c++
A plug-in to the JCCL Config Manager written in C++ that provides the
functionality needed for remote run-time reconfiguration through CORBA.

%package -n jccl-java
Summary: The JCCL Java API
Version: %{jccl_version}
Release: %{jccl_release}
URL: http://www.vrjuggler.org/jccl/
Group: Development/Java
Requires: j2sdk >= %{min_jdk}
Requires: tweek-java = %{tweek_version}-%{tweek_release}
Provides: jccl-java = %{jccl_version}-%{jccl_release}

%description -n jccl-java
The JCCL Java API provides a collection of JavaBeans that are used for
reading and editing JCCL configuration (.jconf) files and config
definition (.jdef) files. The JavaBeans are designed for use with and
depend on the Tweek Java GUI.

%package -n jccl-rtrc-plugin-java
Summary: The JCCL Remote Run-Time Reconfiguration JavaBean
Version: %{jccl_version}
Release: %{jccl_release}
URL: http://www.vrjuggler.org/jccl/
Group: Development/C++
Requires: j2sdk >= %{min_jdk}
Requires: tweek-java = %{tweek_version}-%{tweek_release}
Provides: jccl-rtrc-plugin-java = %{jccl_version}-%{jccl_release}

%description -n jccl-rtrc-plugin-java
A plug-in to the JCCL Config Manager written in C++ that provides the
functionality needed for remote run-time reconfiguration through CORBA.

%package -n sonix
Summary: Sonix
Version: %{sonix_version}
Release: %{sonix_release}
URL: http://www.vrjuggler.org/sonix/
Group: Development/C++
Requires: vpr = %{vpr_version}-%{vpr_release}
BuildRequires: gmtl >= 0.4.5
Provides: sonix = %{sonix_version}-%{sonix_release}

%description -n sonix
Sonix is a dynamically reconfigurable wrapper around audio APIs.

%package -n sonix-devel
Summary: The Sonix Headers
Version: %{sonix_version}
Release: %{sonix_release}
URL: http://www.vrjuggler.org/sonix/
Group: Development/C++
Requires: vpr-devel = %{vpr_version}-%{vpr_release}
Requires: gmtl >= 0.4.5
Requires: sonix = %{sonix_version}-%{sonix_release}
Provides: sonix-devel = %{sonix_version}-%{sonix_release}

%description -n sonix-devel
The header files and libraries needed for developing programs using Sonix.

%package -n sonix-plugin-audiere
Summary: Sonix Audiere Plug-in
Version: %{sonix_version}
Release: %{sonix_release}
URL: http://www.vrjuggler.org/sonix/
Group: Development/C++
Requires: sonix = %{sonix_version}-%{sonix_release}
Requires: audiere >= 1.9.3
BuildRequires: audiere-devel >= 1.9.3
Provides: sonix-plugin-audiere = %{sonix_version}-%{sonix_release}

%description -n sonix-plugin-audiere
Sonix plug-in that uses Audiere to generate sounds.

%package -n sonix-plugin-openal
Summary: Sonix OpenAL Plug-in
Version: %{sonix_version}
Release: %{sonix_release}
URL: http://www.vrjuggler.org/sonix/
Group: Development/C++
Requires: sonix = %{sonix_version}-%{sonix_release}
Requires: openal >= 0.0.8
Requires: freealut >= 1.0.1
BuildRequires: openal-devel >= 0.0.8
BuildRequires: freealut-devel >= 1.0.1
Provides: sonix-plugin-openal = %{sonix_version}-%{sonix_release}

%description -n sonix-plugin-openal
Sonix plug-in that uses OpenAL to generate sounds.

%package -n gadgeteer
Summary: Gadgeteer and all its device drivers and cluster plug-ins
Version: %{gadgeteer_version}
Release: %{gadgeteer_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: libX11.so.6
Requires: libICE.so.6
Requires: libSM.so.6
Requires: vpr = %{vpr_version}-%{vpr_release}
Requires: jccl-c++ = %{jccl_version}-%{jccl_release}
BuildRequires: gmtl >= 0.4.5
Provides: gadgeteer = %{gadgeteer_version}-%{gadgeteer_release}

%description -n gadgeteer
Gadgeteer is an extensible, highly configurable, cluster-ready input
device management for use with virtual reality applications. Users can
write Device drivers that are loaded dynamically.

%package -n gadgeteer-devel
Summary: The Gadgeteer Headers
Version: %{gadgeteer_version}
Release: %{gadgeteer_release}
URL: http://www.vrjuggler.org/gadgeteer/
Group: Development/C++
Requires: gmtl >= 0.4.5
Requires: vpr-devel = %{vpr_version}-%{vpr_release}
Requires: jccl-c++-devel = %{jccl_version}-%{jccl_release}
Requires: gadgeteer = %{gadgeteer_version}-%{gadgeteer_release}
Provides: gadgeteer-devel = %{gadgeteer_version}-%{gadgeteer_release}

%description -n gadgeteer-devel
The header files and libraries needed for developing programs and device
drivers using Gadgeteer. This is required for writing device driver
and cluster plug-ins to Gadgeteer.

%package -n vrjuggler-devel
Summary: The VR Juggler Headers
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: vpr-devel = %{vpr_version}-%{vpr_release}
Requires: jccl-c++-devel = %{jccl_version}-%{jccl_release}
Requires: sonix-devel = %{sonix_version}-%{sonix_release}
Requires: gadgeteer-devel = %{gadgeteer_version}-%{gadgeteer_release}
Requires: vrjuggler = %{vrjuggler_version}-%{vrjuggler_release}
Provides: vrjuggler-devel = %{vrjuggler_version}-%{vrjuggler_release}

%description -n vrjuggler-devel
The header files and libraries needed for developing VR applications using
VR Juggler.

%package -n vrjuggler-opengl
Summary: The VR Juggler OpenGL Draw Manager
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: libGL.so.1
Requires: libGLU.so.1
Requires: libXxf86vm.so.1
Requires: libXext.so.6
Requires: vrjuggler = %{vrjuggler_version}-%{vrjuggler_release}
Provides: vrjuggler-opengl = %{vrjuggler_version}-%{vrjuggler_release}

%description -n vrjuggler-opengl
The VR Juggler OpenGL Draw Manager, which is needed for running applications
based on OpenGL, OpenSG, and Open Scene Graph.

%package -n vrjuggler-opengl-devel
Summary: The VR Juggler OpenGL Draw Manager Headers
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: vrjuggler-opengl = %{vrjuggler_version}-%{vrjuggler_release}
Requires: vrjuggler-devel = %{vrjuggler_version}-%{vrjuggler_release}
Provides: vrjuggler-opengl-devel = %{vrjuggler_version}-%{vrjuggler_release}

%description -n vrjuggler-opengl-devel
The header files and libraries needed for compiling VR Juggler applications
that use the VR Juggler OpenGL Draw Manager. Such applications use OpenGL,
OpenSG, or Open Scene Graph.

%package -n vrjuggler-performer
Summary: The VR Juggler OpenGL Performer Draw Manager
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: vrjuggler = %{vrjuggler_version}-%{vrjuggler_release}
Provides: vrjuggler-performer = %{vrjuggler_version}-%{vrjuggler_release}

%description -n vrjuggler-performer
The VR Juggler OpenGL Performer Draw Manager, which is needed for running
applications based on OpenGL Performer.

%package -n vrjuggler-performer-devel
Summary: The VR Juggler OpenGL Performer Draw Manager Headers
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: vrjuggler-performer = %{vrjuggler_version}-%{vrjuggler_release}
Requires: vrjuggler-devel = %{vrjuggler_version}-%{vrjuggler_release}
Provides: vrjuggler-performer-devel = %{vrjuggler_version}-%{vrjuggler_release}

%description -n vrjuggler-performer-devel
The header files and libraries needed for compiling VR Juggler applications
that use the VR Juggler OpenGL Performer Draw Manager. Such applications use
OpenGL Performer.

%package -n vrjconfig
Summary: VRJConfig, the VR Juggler configuration editor
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/Java
Requires: j2sdk >= %{min_jdk}
Requires: tweek-java = %{tweek_version}-%{tweek_release}
Requires: jccl-java = %{jccl_version}-%{jccl_release}
Provides: vrjconfig = %{vrjuggler_version}-%{vrjuggler_release}

%description -n vrjconfig
VRJConfig is a Java-based GUI for editing VR Juggler configurations. It
is actually a JavaBean that plugs into the Tweek Java GUI.

%package -n vrjuggler-perf-plugin-c++
Summary: The VR Juggler Remote Run-Time Performance Monitoring C++ Plug-in
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: tweek-c++ = %{tweek_version}-%{tweek_release}
Requires: vrjuggler = %{vrjuggler_version}-%{vrjuggler_release}
Provides: vrjuggler-perf-plugin-c++ = %{vrjuggler_version}-%{vrjuggler_release}

%description -n vrjuggler-perf-plugin-c++
A plug-in to the VR Juggler Performance Mediator written in C++ that provides
the functionality needed for remote run-time application performance
monitoring through CORBA.

%package -n vrjuggler-perf-plugin-java
Summary: The VR Juggler Remote Run-Time Performance Monitoring JavaBean
Version: %{vrjuggler_version}
Release: %{vrjuggler_release}
URL: http://www.vrjuggler.org/vrjuggler/
Group: Development/C++
Requires: j2sdk >= %{min_jdk}
Requires: tweek-java = %{tweek_version}-%{tweek_release}
Provides: vrjuggler-perf-plugin-java = %{vrjuggler_version}-%{vrjuggler_release}

%description -n vrjuggler-perf-plugin-java
A plug-in to the VR Juggler Performance Mediator written in Java that
provides the functionality needed for remote run-time application performance
monitoring through CORBA.

%ifarch x86_64
%define abi_option --with-abi=ELF_x86_64
%else
%ifarch i386
%define abi_option --with-abi=ELF_i386
%endif
%endif

%package -n doozer
Summary: Doozer
Version: 2.0.0
Release: 1
URL: http://sourceforge.net/projects/doozer/
Group: Development

%description -n doozer
A build system build on top of GNU make aimed at the creation of simple,
cross-platform makefiles.

%prep
rm -rf %{buildroot}
# For now don't keep unpacking
%setup -q -n %{name}-%{global_version}-%{global_release}.src
#%setup -DT -q -n %{name}-%{global_version}-%{global_release}.src
./configure.pl --with-boost=/usr --with-boost-includes=/usr/include     \
               --with-gmtl=/usr --with-openal=/usr --with-audiere=/usr  \
               --prefix=%{_prefix} %{abi_option}

%build
make BUILD_PROF_C=N BUILD_PROF_CXX=N build

%install
make BUILD_PROF_C=N BUILD_PROF_CXX=N install prefix=%{buildroot}%{_prefix}
# Just in case... stupid scons
find %{buildroot}%{_prefix} -name .sconsign -exec rm {} \;

# The following is to change tweek-base.sh so that it does not depend on
# tweek-config. The reason that it uses tweek-config is so that the Tweek
# installation can be moved around. Since this is being installed through a
# package, we do not need to worry about that use case.
tweek_jars=`%{buildroot}%{_bindir}/tweek-config --prefix=%{_prefix} --jars`
cat %{buildroot}%{_bindir}/tweek-base.sh | \
 sed -e 's|if test ! -x "$TWEEK_BASE_DIR/bin/tweek-config".*|if ! true; then|' \
     -e "s|\`\$TWEEK_BASE_DIR/bin/tweek-config --jars\`|\"$tweek_jars\"|" \
   > %{buildroot}%{_bindir}/tweek-base.sh.tmp
mv %{buildroot}%{_bindir}/tweek-base.sh.tmp %{buildroot}%{_bindir}/tweek-base.sh

%clean
rm -rf %{buildroot}

%post -n vpr
/sbin/ldconfig

%post -n tweek-c++
/sbin/ldconfig

%post -n jccl-c++
/sbin/ldconfig

%post -n sonix
/sbin/ldconfig

%post -n gadgeteer
/sbin/ldconfig

%post -n vrjuggler
/sbin/ldconfig

%post -n vrjuggler-opengl
/sbin/ldconfig

%post -n vrjuggler-performer
/sbin/ldconfig

%postun -n vpr
/sbin/ldconfig

%postun -n tweek-c++
/sbin/ldconfig

%postun -n jccl-c++
/sbin/ldconfig

%postun -n sonix
/sbin/ldconfig

%postun -n gadgeteer
/sbin/ldconfig

%postun -n vrjuggler
/sbin/ldconfig

%postun -n vrjuggler-opengl
/sbin/ldconfig

%postun -n vrjuggler-performer
/sbin/ldconfig

%files -n vpr
%defattr(-, root, root)
%{_libdir}/libvpr.so.*
%{_libdir}/%{vj_arch}/libvpr.so.*
%{_libdir}/%{vj_arch}/*/libvpr.so.*
%doc COPYING.txt modules/vapor/ChangeLog

%files -n vpr-devel
%defattr(-, root, root)
%{_bindir}/vpr-config
%{_includedir}/vpr
%{_libdir}/libvpr.a
%{_libdir}/%{vj_arch}/libvpr.a
%{_libdir}/%{vj_arch}/*/libvpr.a
%{_libdir}/libvpr.so
%{_libdir}/%{vj_arch}/libvpr.so
%{_libdir}/%{vj_arch}/*/libvpr.so
%{_libdir}/flagpoll/boost*.fpc
%{_libdir}/flagpoll/vpr*.fpc
%{_prefix}/share/aclocal/vpr.m4
%{_prefix}/share/aclocal/vrj-helpers.m4
%{_prefix}/share/vpr

%if %have_omniorb
%files -n tweek-c++
%defattr(-, root, root)
%{_libdir}/libtweek.so.*
%{_libdir}/%{vj_arch}/libtweek.so.*
%{_libdir}/%{vj_arch}/*/libtweek.so.*
%{_prefix}/share/tweek/data
%{_prefix}/share/tweek/LICENSE
%{_prefix}/share/tweek/README.source
%doc COPYING.txt modules/tweek/ChangeLog

%files -n tweek-c++-devel
%defattr(-, root, root)
%{_bindir}/tweek-config
%{_includedir}/tweek
%{_libdir}/libtweek.a
%{_libdir}/%{vj_arch}/libtweek.a
%{_libdir}/%{vj_arch}/*/libtweek.a
%{_libdir}/libtweek.so
%{_libdir}/%{vj_arch}/libtweek.so
%{_libdir}/%{vj_arch}/*/libtweek.so
%{_libdir}/flagpoll/tweek*.fpc
%{_prefix}/share/aclocal/tweek.m4
%{_prefix}/share/tweek/test
%{_prefix}/share/tweek/tweek.appdefs.mk
%endif

%if %have_java
%files -n tweek-java
%defattr(-, root, root)
%{_bindir}/idl
%{_bindir}/idl.jar
%{_bindir}/logkit-1.2.jar
%{_bindir}/tweek
%{_bindir}/tweek-base.sh
%{_prefix}/share/tweek/beans
%{_prefix}/share/tweek/java
%doc COPYING.txt modules/tweek/ChangeLog
%endif

%files -n jccl-c++
%defattr(-, root, root)
%{_libdir}/libjccl.so.*
%{_libdir}/%{vj_arch}/libjccl.so.*
%{_libdir}/%{vj_arch}/*/libjccl.so.*
%{_prefix}/share/jccl/data
%{_prefix}/share/jccl/tools
%doc COPYING.txt modules/jackal/ChangeLog

%files -n jccl-c++-devel
%defattr(-, root, root)
%{_bindir}/jccl-config
%{_includedir}/jccl
%{_libdir}/libjccl.a
%{_libdir}/%{vj_arch}/libjccl.a
%{_libdir}/%{vj_arch}/*/libjccl.a
%{_libdir}/libjccl.so
%{_libdir}/%{vj_arch}/libjccl.so
%{_libdir}/%{vj_arch}/*/libjccl.so
%{_libdir}/flagpoll/jccl*.fpc
%{_prefix}/share/aclocal/jccl.m4
%{_prefix}/share/jccl/test

%if %have_omniorb
%files -n jccl-rtrc-plugin-c++
%defattr(-, root, root)
%{_libdir}/jccl
%endif

%if %have_java
%files -n jccl-java
%defattr(-, root, root)
%{_prefix}/share/jccl/beans/jccl_config.*
%{_prefix}/share/jccl/beans/jccl_editors.*
%{_prefix}/share/jccl/java
%doc COPYING.txt modules/jackal/ChangeLog

%files -n jccl-rtrc-plugin-java
%defattr(-, root, root)
%{_prefix}/share/jccl/beans/jccl_rtrc.*
%endif

%files -n sonix
%defattr(-, root, root)
%{_libdir}/libsonix.so.*
%{_libdir}/%{vj_arch}/libsonix.so.*
%{_libdir}/%{vj_arch}/*/libsonix.so.*
%{_prefix}/share/sonix/data
%doc COPYING.txt modules/sonix/ChangeLog

%files -n sonix-devel
%defattr(-, root, root)
%{_bindir}/sonix-config
%{_includedir}/snx
%{_libdir}/libsonix.a
%{_libdir}/%{vj_arch}/libsonix.a
%{_libdir}/%{vj_arch}/*/libsonix.a
%{_libdir}/libsonix.so
%{_libdir}/%{vj_arch}/libsonix.so
%{_libdir}/%{vj_arch}/*/libsonix.so
%{_libdir}/flagpoll/sonix*.fpc
%{_prefix}/share/aclocal/sonix.m4
%{_prefix}/share/sonix/samples

%if %have_audiere
%files -n sonix-plugin-audiere
%defattr(-, root, root)
%{_libdir}/snx/plugins/*/Audiere_snd.so
%endif

%if %have_openal
%files -n sonix-plugin-openal
%defattr(-, root, root)
%{_libdir}/snx/plugins/*/OpenAL_snd.so
%endif

%files -n gadgeteer
%defattr(-, root, root)
%{_libdir}/libgadget.so.*
%{_libdir}/%{vj_arch}/libgadget.so.*
%{_libdir}/%{vj_arch}/*/libgadget.so.*
%{_libdir}/gadgeteer
%{_prefix}/share/gadgeteer/data
%{_prefix}/share/gadgeteer/tools
%doc COPYING.txt modules/gadgeteer/ChangeLog

%files -n gadgeteer-devel
%defattr(-, root, root)
%{_bindir}/gadgeteer-config
%{_includedir}/cluster
%{_includedir}/gadget
%{_includedir}/plugins
%{_libdir}/libgadget.a
%{_libdir}/%{vj_arch}/libgadget.a
%{_libdir}/%{vj_arch}/*/libgadget.a
%{_libdir}/libgadget.so
%{_libdir}/%{vj_arch}/libgadget.so
%{_libdir}/%{vj_arch}/*/libgadget.so
%{_libdir}/flagpoll/gadgeteer*.fpc
%{_prefix}/share/aclocal/gadget.m4
%{_prefix}/share/gadgeteer/*.mk
%{_prefix}/share/gadgeteer/samples
%{_prefix}/share/gadgeteer/test

%files
%defattr(-, root, root)
%{_libdir}/libvrj.so.*
%{_libdir}/%{vj_arch}/libvrj.so.*
%{_libdir}/%{vj_arch}/*/libvrj.so.*
%{_prefix}/share/vrjuggler/data
%{_prefix}/share/vrjuggler/tools
%doc COPYING.txt INSTALL.html INSTALL.txt README.html README.txt modules/vrjuggler/ChangeLog modules/vrjuggler/RELEASE_NOTES.txt

%files -n vrjuggler-devel
%defattr(-, root, root)
%{_bindir}/vrjuggler-config
%{_includedir}/deprecated
%{_includedir}/vrj/*.h
%{_includedir}/vrj/Display
%{_includedir}/vrj/Draw/*.h
%{_includedir}/vrj/Kernel
%{_includedir}/vrj/Performance
%{_includedir}/vrj/Sound
%{_includedir}/vrj/Test
%{_includedir}/vrj/Util
%{_libdir}/libvrj.a
%{_libdir}/%{vj_arch}/libvrj.a
%{_libdir}/%{vj_arch}/*/libvrj.a
%{_libdir}/libvrj.so
%{_libdir}/%{vj_arch}/libvrj.so
%{_libdir}/%{vj_arch}/*/libvrj.so
%{_libdir}/flagpoll/vrjuggler*.fpc
%{_prefix}/share/aclocal/vrj.m4
%{_prefix}/share/vrjuggler/*.mk
%{_prefix}/share/vrjuggler/samples
%{_prefix}/share/vrjuggler/test

%files -n vrjuggler-opengl
%defattr(-, root, root)
%{_libdir}/libvrj_ogl.so.*
%{_libdir}/%{vj_arch}/libvrj_ogl.so.*
%{_libdir}/%{vj_arch}/*/libvrj_ogl.so.*

%files -n vrjuggler-opengl-devel
%defattr(-, root, root)
%{_includedir}/vrj/Draw/OGL
%{_includedir}/vrj/Draw/OSG
%{_includedir}/vrj/Draw/OpenSG
%{_libdir}/libvrj_ogl.a
%{_libdir}/%{vj_arch}/libvrj_ogl.a
%{_libdir}/%{vj_arch}/*/libvrj_ogl.a
%{_libdir}/libvrj_ogl.so
%{_libdir}/%{vj_arch}/libvrj_ogl.so
%{_libdir}/%{vj_arch}/*/libvrj_ogl.so

%if %have_performer
%files -n vrjuggler-performer
%defattr(-, root, root)
%{_libdir}/libvrj_pf.so.*
%{_libdir}/%{_arch}/libvrj_pf.so.*
%{_libdir}/%{_arch}/*/libvrj_pf.so.*

%files -n vrjuggler-performer-devel
%defattr(-, root, root)
%{_includedir}/vrj/Draw/Pf
%{_libdir}/libvrj_pf.a
%{_libdir}/%{vj_arch}/libvrj_pf.a
%{_libdir}/%{vj_arch}/*/libvrj_pf.a
%{_libdir}/libvrj_pf.so
%{_libdir}/%{vj_arch}/libvrj_pf.so
%{_libdir}/%{vj_arch}/*/libvrj_pf.so
%endif

%if %have_java
%files -n vrjconfig
%defattr(-, root, root)
%{_bindir}/vrjconfig
%{_prefix}/share/vrjuggler/beans/customeditors
%{_prefix}/share/vrjuggler/beans/wizards
%{_prefix}/share/vrjuggler/beans/VRJConfig.*
%{_prefix}/share/vrjuggler/java/CommonEditors.jar
%{_prefix}/share/vrjuggler/java/jgraphaddons.jar
%{_prefix}/share/vrjuggler/java/jgraph.jar
%doc COPYING.txt modules/vrjuggler/vrjconfig/LICENSE-java-laf.txt
%endif

%if %have_omniorb
%files -n vrjuggler-perf-plugin-c++
%defattr(-, root, root)
%{_libdir}/vrjuggler
%endif

%if %have_java
%files -n vrjuggler-perf-plugin-java
%defattr(-, root, root)
%{_prefix}/share/vrjuggler/beans/PerformanceMonitor.*
%{_prefix}/share/vrjuggler/java/gnujaxp.jar
%{_prefix}/share/vrjuggler/java/jcommon.jar
%{_prefix}/share/vrjuggler/java/jfreechart.jar
%{_prefix}/share/vrjuggler/java/servlet.jar
%endif

%files -n doozer
%{_prefix}/share/Doozer

%changelog
* Thu Apr 14 2006 Patrick Hartling
- Split into sub-packages.
- Allow for optional sub-packages depending on the build environment.
- Added support for multi-architecture builds.

* Tue Jun 06 2005 Aron Bierbaum
- Initial version.
