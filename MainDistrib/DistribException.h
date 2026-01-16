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
  InvalidFormat = 1,  // Format de donnée invalide
  InvalidHour = 2     // Heure impossible
};

class DistribException {
  public:
    int exType;       // Type d'exception
    String exMessage; // Message de l'exception

    DistribException(int exceptionType, String exceptionMessage);
    ~DistribException();
};

#endif