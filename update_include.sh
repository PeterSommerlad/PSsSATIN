#!/bin/sh
rm include/*
cd include
ln ../cevelop-workspace/PSsSATINTests/src/psssatin.h .
ln ../cevelop-workspace/PSSATINwithStructTests/src/satins.h .
ls -l