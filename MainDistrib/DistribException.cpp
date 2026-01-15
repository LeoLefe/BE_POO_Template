/*********************************************************************
 * @file  DistribException.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe DistribException
 *********************************************************************/
#include "DistribException.h"

DistribException::DistribException(int exceptionType, String exceptionMessage) {
  exType = exceptionType;
  exMessage = exceptionMessage;
}

DistribException::~DistribException() {}
