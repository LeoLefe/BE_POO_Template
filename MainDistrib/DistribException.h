/*********************************************************************
 * @file  DistribException.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe DistribException
 *********************************************************************/
#ifndef DISTRIBEXCEPTION_H
#define DISTRIBEXCEPTION_H

#include <Arduino.h>

enum ExceptionType
{
  Unknown = 0,
  InvalidFormat = 1,
  InvalidHour = 2,
  InvalidNumberMeals = 3
};

class DistribException {
  public:
    int exType;
    String exMessage;

    DistribException(int exceptionType, String exceptionMessage);
    ~DistribException();
};

#endif