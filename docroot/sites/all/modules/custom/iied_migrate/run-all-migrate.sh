#!/bin/bash
echo 'migrating biblio'
drush migrate-import biblio
echo 'migrating case'
drush migrate-import case
echo 'migrating document'
drush migrate-import document
echo 'migrating event'
drush migrate-import event
echo 'migrating initia'
drush migrate-import initia
echo 'migrating link'
drush migrate-import link
echo 'migrating organ'
drush migrate-import organ
echo '---MIGRATE COMPLETE---'

