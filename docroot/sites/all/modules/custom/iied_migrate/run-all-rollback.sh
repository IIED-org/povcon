#!/bin/bash
echo 'rollback biblio'
drush migrate-rollback biblio
echo 'rollback case'
drush migrate-rollback case
echo 'rollback document'
drush migrate-rollback document
echo 'rollback event'
drush migrate-rollback event
echo 'rollback initia'
drush migrate-rollback initia
echo 'rollback link'
drush migrate-rollback link
echo 'rollback organ'
drush migrate-rollback organ
echo '---rollback COMPLETE---'