#ifndef DISTRIBUTIONMANAGER_H
#define DISTRIBUTIONMANAGER_H

void check_heure_distribution();
void set_heure_distribution_matin(const String& heure);
void set_heure_distribution_soir(const String& heure);
void set_heure_reset_matin(const String& heure);
void set_heure_reset_soir(const String& heure);
void distrib_manuelle(int portions_manuelles);

#endif // DISTRIBUTIONMANAGER_H