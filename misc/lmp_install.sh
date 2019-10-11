git clone https://github.com/lammps/lammps.git -b stable --depth 1
mkdir lammps/build

cd lammps/src
USER-OMP/hack_openmp_for_pgi_gcc9.sh
cd USER-OMP
./hack_openmp_for_pgi_gcc9.sh
cd ../USER-INTEL
../USER-OMP/hack_openmp_for_pgi_gcc9.sh
cd ../../build

cmake \
\
-D CMAKE_BUILD_TYPE=Release \
-D PKG_OPT=on \
-D BUILD_OMP=on \
-D BUILD_MPI=on \
-D PKG_KSPACE=on \
-D PKG_MOLECULE=on \
-D PKG_USER-REAXC=on \
-D PKG_MANYBODY=on \
-D PKG_QEQ=on \
-D PKG_USER-OMP=on \
-D PKG_USER-INTEL=on \
-D WITH_JPEG=yes \
-D WITH_PNG=yes \
-D WITH_FFMPEG=yes \
-D PKG_KOKKOS=on \
-D KOKKOS_ARCH=HSW \
-D KOKKOS_ENABLE_OPENMP=yes \
\
../cmake

cmake --build . --target install
