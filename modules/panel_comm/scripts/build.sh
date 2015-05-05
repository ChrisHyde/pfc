#!/bin/bash
INSTALLMAININCLUDEPATH="/usr/local/include/pfc/"

INSTALLMAINLIBPATH="/usr/local/lib/pfc/"

GENERICINCLUDEPATH="../../include"

MAKEOUTPUT="/dev/null"
returnValue="0"

compiledependence(){
if test "$returnValue" = "0"
then
 echo $1
 echo "-----------------------------------"
 cd $1
 make clean 
 make OS=XENOMAI
 if test "$?" = "0"
 then
  echo "OK"
  echo "--------------Install Includes----------------"
  cd include
  echo $PWD
  sudo cp *.h $INSTALLMAININCLUDEPATH
  cd ..
  
  echo "---------------Install Libs-------------------"
  echo $PWD
  sudo cp lib/lib* $INSTALLMAINLIBPATH


   cd ../include
   echo $PWD
  sudo cp *.h $INSTALLMAININCLUDEPATH

  cd ../xplane_comm/scripts
echo "---------------End-------------------"
 else
  echo "NOK"
  returnValue=-1
 fi

fi
}



echo "#------------------------------------#"
echo "# xplane_comm SOFTWARE BUILD PROCESS #"
echo "#------------------------------------#"



  sudo mkdir -p /usr/local/include/pfc

  sudo mkdir -p /usr/local/lib/pfc




echo "1. compile project"
echo "------------------"

cd ../../

compiledependence "xplane_comm"

 

echo "5. end"
echo "-----------------------------------"
sudo ln -s -f /usr/local/lib/pfc/libxplane_commd.so /usr/lib/libxplane_commd.so
sudo ldconfig
if test "$returnValue" = "0"
then
 echo "build
 echo successfully completed!"
else
 echo "there were errors on process..."
fi

