<?php
abstract class IiedMigration extends Migration {
  public function __construct() {
    parent::__construct();

    $this->team = array(
      new MigrateTeamMember('Joe Mewes', 'joe@4alldigital.com', t('Implementor')),
    );
  }
}

class BiblioMigration extends IiedMigration {

	
	//SQL
	//UPDATE `Biblio` SET `Author` = SUBSTR(`Author`, 2)
	//UPDATE `Biblio` SET `GeoCoverage` = SUBSTR(`GeoCoverage`, 2)
	//UPDATE `Biblio` SET `AuthorOrg` = SUBSTR(`AuthorOrg`, 2)
	
	
  public function __construct() {
		
    parent::__construct();

    $this->description = t('Migration of a bibliographies');

    $columns = array
    (
      array('id',          'Unique identifier'),
	  array('idnum',          'OLD bespoke ID'),
      array('Authorname',    'Author name'),
      array('MonthYear',    'Date'),
      array('Title',    'Title'),
      array('ReferenceType',    'Reference'),
      array('SourceName',    'Source Name'),
      array('SourceEditor',    'Source Editor'),
	  array('JournalRef',    'Journal Reference'),
	  array('JournalNum',    'Journal Number'),
      array('JournalVol',    'Journal Volume'),
      array('PageNumbers',    'Page Numbers'),
      array('Publisher',    'Publisher'),
      array('Summary',    'Summary'),
      array('Themes',    'Themes'),
      array('GeoCoverage',    'Geo Coverage'),
      array('AvailableFrom',    'Available From URL'),
      array('DOI',    'DOI code'),
      array('AuthorOrg',    'Organisation of author'),
      array('Keywords',    'Keywords'),
      array('Problems',    'Problems'),
      array('InternalNotes',    'Internal Notes'),
	  array('rel_link_url',    'Related link'),
	  array('rel_link_title',    'Related link title'),
    );

    $csv_file = drupal_get_path('module', 'iied_migrate').'/biblio.csv';
    $this->source = new MigrateSourceCSV($csv_file, $columns);

    $this->destination = new MigrateDestinationNode('bibliography', array('text_format' => 'full_html'));

    $this->map = new MigrateSQLMap(
      $this->machineName,
      array
      (
        'id' => array
        (
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('is_new')
         ->defaultValue(TRUE);

    $this->addFieldMapping('uid', 'author')
         ->defaultValue(1);

    $this->addFieldMapping('title', 'Title');
	$this->addFieldMapping('field_idnum', 'idnum');
	$this->addFieldMapping('field_authors', 'Authorname')
		->separator('|');
	$this->addFieldMapping('field_month_year', 'MonthYear');
	$this->addFieldMapping('field_ref_type', 'ReferenceType');
	$this->addFieldMapping('field_source_name', 'SourceName');
	$this->addFieldMapping('field_source_editor', 'SourceEditor');
	$this->addFieldMapping('field_journal_ref', 'JournalRef');
	$this->addFieldMapping('field_journal_num', 'JournalNum');
	$this->addFieldMapping('field_journal_vol', 'JournalVol');
	$this->addFieldMapping('field_page_num', 'PageNumbers');
	$this->addFieldMapping('field_publisher', 'Publisher');
	$this->addFieldMapping('field_summary', 'Summary');
	$this->addFieldMapping('field_themes', 'Themes')
		->separator('|');
    $this->addFieldMapping('field_geo_cover', 'GeoCoverage')
		->separator('|');
	$this->addFieldMapping('field_doc_id', 'DOI');
	$this->addFieldMapping('field_available', 'AvailableFrom');
	$this->addFieldMapping('field_author_org', 'AuthorOrg')
		->separator('|');
	$this->addFieldMapping('field_keywords', 'Keywords');
	$this->addFieldMapping('field_problems', 'Problems');
	$this->addFieldMapping('field_internal_notes', 'InternalNotes');
	
   // $this->addFieldMapping('field_rel_link[und][0][title]', 'rel_link_title');
	
	
//	$this->addFieldMapping(NULL, 'rel_link_title')
//	->separator('|');	
//	
//	$this->addFieldMapping('field_rel_link', 'rel_link_url')
//		->separator('|');
//	
	
	//IMPORTANT - DO this source mapping without a destination mapping (NULL)
//	$arguments = MigrateFileFieldHandler::arguments(NULL, 'file_copy', FILE_EXISTS_REPLACE);
//	$this->addFieldMapping(NULL, 'rel_link_title')
      //  ->separator('|')
      //  ->description('See prepare().');


	$this->addFieldMapping('field_rel_link', 'rel_link_url')
		->separator('|')
		->description(t('Mapping related links'));
	//	->arguments(array(
      //    'title' => 'See prepare().',
          // Alternatively, specify a column here for dynamic file name.
      //));	
	$this->addFieldMapping('pathauto')->defaultValue(TRUE);
$this->addFieldMapping('status')->defaultValue(TRUE);

  }

	public function prepareRow($current_row) {
		
		
	$replacement = "1";
		
		
    //do the times    
    $article_date = $current_row->MonthYear;	

		$length = strlen($article_date);

	//echo $length;

	if($length == 1){
		$article_date = '200001';
	}

		if(substr($article_date, -1) == 0){
			$article_date = substr($article_date, 0, -1).$replacement; 
		};
		

	$parts = strptime( $article_date, "%Y%m");
	$year = $parts['tm_year'] + 1900; // 1895
	$month = $parts['tm_mon'] + 1; // 10
	
	//echo $article_date;
	
	$current_row->MonthYear = $year."-".$month; 
	
	
	
	
	if($current_row->DOI != NULL){ 
	$current_row->DOI = 'http://dx.doi.org/' . $current_row->DOI;
	}else{
		$current_row->DOI = '';
	}
	
	
	
	
	
	

  }


	public function prepare($current_row, $row) 
	{   
		 if (!isset($current_row->field_rel_link['und'][0]['title'])) 
		    {
				$result = $row->rel_link_title;
				$result = explode('|', $result);
				foreach ($current_row->field_rel_link['und'] as $key => $item)
				{
					$current_row->rel_link_title = substr($current_row->rel_link_title, 2);
					$current_row->field_rel_link['und'][$key]['title'] = $result[$key];
				}    
			}
		
		
		//print_r($current_row->field_rel_link);
			
		//	$URL = $current_row->field_rel_link['und'][$key]['url'];
			if (isset($current_row->field_rel_link['und'][0]['url'])) 
			    {
			//	echo '1';
					$URL = $row->rel_link_url;
				//	echo $URL;
					$URL = explode('|', $URL);
				//	print_r($URL);
					foreach ($current_row->field_rel_link['und'] as $key => $item)
					{
				//	echo $URL[$key].'\n';
					//	$current_row->rel_link_url = substr($URL[$key], 0, 1);
				
				//	echo $current_row->rel_link_url;
					//	$current_row->field_rel_link['und'][$key]['url'] = $URL[$key];
						switch(substr($URL[$key], 0, 1)){
							case 'O':
								echo 'org';
							//	$current_row->rel_link_url = 'en/org/'.$current_row->rel_link_url;
								$current_row->field_rel_link['und'][$key]['url'] = 'en/org/'.	$current_row->field_rel_link['und'][$key]['url'];
								echo 'org DONE';
							break;
								case 'B':
								echo 'bib';
									$current_row->field_rel_link['und'][$key]['url'] = 'en/bib/'.	$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'D':
								echo 'Doc';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/doc/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'C':
								echo 'Doc';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/case/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'E':
								echo 'Event';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/event/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'N':
								echo '';
								$current_row->field_rel_link['und'][$key]['url'] = '';
								break;	
							case '0':
								echo '';
								$current_row->field_rel_link['und'][$key]['url'] = '';
								break;
						}					
					}
						    
				}			
		
  	}
}


class CasestudyMigration extends IiedMigration {
  public function __construct() {
    parent::__construct();

    $this->description = t('Migration of a Case studies');

    $columns = array
    (
      array('id',          'Unique identifier'),
	  array('idnum',          'OLD bespoke ID'),
      array('MonthYear',    'Date'),
      array('Title',    'Title'),
      array('Agency',    'Agency'),
      array('DonorTech',    'DonorTech'),
      array('ImpSelf',    'ImpSelf'),
      array('Context',    'Context'),
	  array('Geocoverage',    'Geo Coverage'),
	  array('Locality',    'Locality'),
      array('BioFocus',    'Bio Focus'),
      array('DevFocus',    'Dev Focus'),
      array('ConsGoals',    'Cons Goals'),
      array('PovGoals',    'Pov Goals'),
      array('PolicyContext',    'Policy Context'),
      array('Summary',    'Summary'),
      array('ConsImpact',    'Cons Impact'),
      array('PovImpact',    'Pov Impact'),
      array('Strategy',    'Strategy'),
      array('Ref1',    'Ref 1'),
      array('Ref2',    'Ref 2'),
      array('MoreInfo',    'More Info'),
      array('Keywords',    'Keywords'),
      array('Problems',    'Problems'),
      array('InternalNotes',    'Internal Notes'),
	  array('rel_link_url',    'Related link'),
	  array('rel_link_title',    'Related link title'),
    );

    $csv_file = drupal_get_path('module', 'iied_migrate').'/case.csv';
    $this->source = new MigrateSourceCSV($csv_file, $columns);

    $this->destination = new MigrateDestinationNode('case_study', array('text_format' => 'full_html'));

    $this->map = new MigrateSQLMap(
      $this->machineName,
      array
      (
        'id' => array
        (
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('is_new')
         ->defaultValue(TRUE);

    $this->addFieldMapping('uid', 'author')
         ->defaultValue(1);

    $this->addFieldMapping('title', 'Title');
	$this->addFieldMapping('field_idnum', 'idnum');
	$this->addFieldMapping('field_cons_context', 'Context')->separator('|');
	$this->addFieldMapping('field_month_year', 'MonthYear');
	$this->addFieldMapping('field_agency', 'Agency');
	$this->addFieldMapping('field_donor_tech', 'DonorTech')
	->separator('|');
	$this->addFieldMapping('field_imp_self', 'ImpSelf')->separator('|');
	$this->addFieldMapping('field_geo_cover', 'Geocoverage')
	->separator('|')
	->defaultValue(2);
	$this->addFieldMapping('field_locality', 'Locality');
	$this->addFieldMapping('field_biofocus', 'BioFocus');
	$this->addFieldMapping('field_devfocus', 'DevFocus');
	$this->addFieldMapping('field_consgoals', 'ConsGoals');
	$this->addFieldMapping('field_povgoals', 'PovGoals');
    $this->addFieldMapping('field_summary', 'Summary');
	$this->addFieldMapping('field_consimpact', 'ConsImpact');
	$this->addFieldMapping('field_povimpact', 'PovImpact');
	$this->addFieldMapping('field_policy_context', 'PolicyContext');
	$this->addFieldMapping('field_strategy', 'Strategy')->separator('|');
	$this->addFieldMapping('field_ref1', 'Ref1');
	$this->addFieldMapping('field_ref2', 'Ref2');
	$this->addFieldMapping('field_moreinfo', 'MoreInfo');
	$this->addFieldMapping('field_keywords', 'Keywords');
	$this->addFieldMapping('field_problems', 'InternalNotes');
	$this->addFieldMapping('field_internal_notes', 'InternalNotes');
	
	
	
	$this->addFieldMapping('field_rel_link', 'rel_link_url')
		->separator('|')
		->description(t('Mapping related links'));
	
	
	$this->addFieldMapping('pathauto')->defaultValue(TRUE);
$this->addFieldMapping('status')->defaultValue(TRUE);


  }

	public function prepareRow($current_row) {
		
		
		//SQL
		//UPDATE `Case` SET `GeoCoverage` = SUBSTR(`GeoCoverage`, 2)
		//UPDATE `Case` SET `DonorTech` = SUBSTR(`DonorTech`, 2)
		//UPDATE `Case` SET `Strategy` = SUBSTR(`Strategy`, 2)
		
	//echo '$current_row';
    //do the times    
   //do the times

    	//this replaces a last 0 monthvalue with a 1
		$replacement = "1";
	
    	$article_date = $current_row->MonthYear;
		$length = strlen($article_date);
	
	//echo $length;
	
	if($length == 1){
		$article_date = '200001';
	}
		
		if(substr($article_date, -1) == 0){
			$article_date = substr($article_date, 0, -1).$replacement; 
		};
		//echo $article_date;
		
		
	$parts = strptime( $article_date, "%Y%m");
	
	$year = $parts['tm_year'] + 1900; // 1895
	
//	echo $year;
	$month = $parts['tm_mon'] + 1; // 10

	$current_row->MonthYear = $year."-".$month;
	
  }


	public function prepare($current_row, $row) 
	{   
		 if (!isset($current_row->field_rel_link['und'][0]['title'])) 
		    {
				$result = $row->rel_link_title;
				$result = explode('|', $result);
				foreach ($current_row->field_rel_link['und'] as $key => $item)
				{
					$current_row->rel_link_title = substr($current_row->rel_link_title, 2);
					$current_row->field_rel_link['und'][$key]['title'] = $result[$key];
				}    
			}
		
		
		//print_r($current_row->field_rel_link);
			
		//	$URL = $current_row->field_rel_link['und'][$key]['url'];
			if (isset($current_row->field_rel_link['und'][0]['url'])) 
			    {
			//	echo '1';
					$URL = $row->rel_link_url;
				//	echo $URL;
					$URL = explode('|', $URL);
				//	print_r($URL);
					foreach ($current_row->field_rel_link['und'] as $key => $item)
					{
				//	echo $URL[$key].'\n';
					//	$current_row->rel_link_url = substr($URL[$key], 0, 1);
				
				//	echo $current_row->rel_link_url;
					//	$current_row->field_rel_link['und'][$key]['url'] = $URL[$key];
						switch(substr($URL[$key], 0, 1)){
							case 'O':
								echo 'org';
							//	$current_row->rel_link_url = 'en/org/'.$current_row->rel_link_url;
								$current_row->field_rel_link['und'][$key]['url'] = 'en/org/'.	$current_row->field_rel_link['und'][$key]['url'];
								echo 'org DONE';
							break;
								case 'B':
								echo 'bib';
									$current_row->field_rel_link['und'][$key]['url'] = 'en/bib/'.	$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'D':
								echo 'Doc';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/doc/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'C':
								echo 'Doc';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/case/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'E':
								echo 'Event';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/event/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'N':
								echo '';
								$current_row->field_rel_link['und'][$key]['url'] = '';
								break;	
							case '0':
								echo '';
								$current_row->field_rel_link['und'][$key]['url'] = '';
								break;
						}					
					}
						    
				}			
		
  	}
    
  
}



class OrganMigration extends IiedMigration {
	
	
	//SQL
	//UPDATE `Organ` SET `OrgInterest` = SUBSTR(`OrgInterest`, 2)
	//UPDATE `Organ` SET `GeoCoverage` = SUBSTR(`GeoCoverage`, 2)
	
	
  public function __construct() {
    parent::__construct();

    $this->description = t('Migration of a organisations');

    $columns = array
    (
      array('id',          'Unique identifier'),
	  array('idnum',          'OLD bespoke ID'),
      array('OrgName',    'Organisation Name'),
      array('OrgQualify',    'Org Qualify'),
      array('ContactDetails',    'Contact Details'),
      array('OrgType',    'Organisation Type'),
      array('OrgInterest',    'Organisation Interest'),
      array('Location',    'Location'),
	  array('HasPage',    'Has a Page'),
	  array('IsGroupMember',    'Is a GroupMember'),
      array('Description',    'Description'),
      array('Projects',    'Projects'),
      array('GeoCoverage',    'GeoCoverage'),
      array('People',    'People'),
      array('WebURL',    'WebURL'),
      array('Keywords',    'Keywords'),
      array('Problems',    'Problems'),
      array('InternalNotes',    'InternalNotes'),
	  array('rel_link_url',    'Related link'),
	  array('rel_link_title',    'Related link title'),
    );

    $csv_file = drupal_get_path('module', 'iied_migrate').'/organ.csv';
    $this->source = new MigrateSourceCSV($csv_file, $columns);

    $this->destination = new MigrateDestinationNode('organisations', array('text_format' => 'full_html'));

    $this->map = new MigrateSQLMap(
      $this->machineName,
      array
      (
        'id' => array
        (
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('is_new')
         ->defaultValue(TRUE);

    $this->addFieldMapping('uid', 'author')
         ->defaultValue(1);

    $this->addFieldMapping('title', 'OrgName');
	$this->addFieldMapping('field_idnum', 'idnum');
	$this->addFieldMapping('field_org_name', 'OrgName');
	$this->addFieldMapping('field_org_qualify', 'OrgQualify');
	$this->addFieldMapping('field_contact_details', 'ContactDetails');
	$this->addFieldMapping('field_org_type', 'OrgType')
		->separator('|');
	$this->addFieldMapping('field_org_interest', 'OrgInterest')
		->separator('|');
	$this->addFieldMapping('field_org_location', 'Location')
		->separator('|');
	$this->addFieldMapping('field_has_page', 'HasPage')
		->separator('|');
	$this->addFieldMapping('field_lg_member', 'IsGroupMember')
		->separator('|');
	$this->addFieldMapping('field_summary', 'Description');
	$this->addFieldMapping('field_projects', 'Projects');
	$this->addFieldMapping('field_geo_cover', 'GeoCoverage')
		->separator('|');
	$this->addFieldMapping('field_people', 'People');
    $this->addFieldMapping('field_weburl', 'WebURL');
	$this->addFieldMapping('field_keywords', 'Keywords');
	$this->addFieldMapping('field_problems', 'Problems');
	$this->addFieldMapping('field_internal_notes', 'InternalNotes');		
	
	
	$this->addFieldMapping('field_rel_link', 'rel_link_url')
		->separator('|')
		->description(t('Mapping related links'));
		
	
	$this->addFieldMapping('pathauto')->defaultValue(TRUE);
$this->addFieldMapping('status')->defaultValue(TRUE);


  }

	public function prepareRow($current_row) {
		
		//prepare func

  }

	public function prepare($current_row, $row) 
	{   
		 if (!isset($current_row->field_rel_link['und'][0]['title'])) 
		    {
				$result = $row->rel_link_title;
				$result = explode('|', $result);
				foreach ($current_row->field_rel_link['und'] as $key => $item)
				{
					$current_row->rel_link_title = substr($current_row->rel_link_title, 2);
					$current_row->field_rel_link['und'][$key]['title'] = $result[$key];
				}    
			}
		
		
		//print_r($current_row->field_rel_link);
			
		//	$URL = $current_row->field_rel_link['und'][$key]['url'];
			if (isset($current_row->field_rel_link['und'][0]['url'])) 
			    {
			//	echo '1';
					$URL = $row->rel_link_url;
				//	echo $URL;
					$URL = explode('|', $URL);
				//	print_r($URL);
					foreach ($current_row->field_rel_link['und'] as $key => $item)
					{
				//	echo $URL[$key].'\n';
					//	$current_row->rel_link_url = substr($URL[$key], 0, 1);
				
				//	echo $current_row->rel_link_url;
					//	$current_row->field_rel_link['und'][$key]['url'] = $URL[$key];
						switch(substr($URL[$key], 0, 1)){
							case 'O':
								echo 'org';
							//	$current_row->rel_link_url = 'en/org/'.$current_row->rel_link_url;
								$current_row->field_rel_link['und'][$key]['url'] = 'en/org/'.	$current_row->field_rel_link['und'][$key]['url'];
								echo 'org DONE';
							break;
								case 'B':
								echo 'bib';
									$current_row->field_rel_link['und'][$key]['url'] = 'en/bib/'.	$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'D':
								echo 'Doc';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/doc/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'C':
								echo 'Doc';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/case/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'E':
								echo 'Event';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/event/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'N':
								echo '';
								$current_row->field_rel_link['und'][$key]['url'] = '';
								break;	
							case '0':
								echo '';
								$current_row->field_rel_link['und'][$key]['url'] = '';
								break;
						}					
					}
						    
				}			
		
  	}
    
  
}



class InitiaMigration extends IiedMigration {
	
	
	//SQL
	//UPDATE `Organ` SET `OrgInterest` = SUBSTR(`OrgInterest`, 2)
	//UPDATE `Organ` SET `GeoCoverage` = SUBSTR(`GeoCoverage`, 2)
	
	
  public function __construct() {
    parent::__construct();

    $this->description = t('Migration of a intiatives');


	//UPDATE `Initia` SET `ImpOrgan` = SUBSTR(`ImpOrgan`, 2)
	//UPDATE `Initia` SET `GeoCoverage` = SUBSTR(`GeoCoverage`, 2)

    $columns = array
    (
      array('id',         	 	'Unique identifier'),
	  array('idnum',         	'OLD bespoke ID'),
      array('title',    	 	'Title'),
      array('ImpOrgan',    	'ImpOrgan'),
      array('HasPage',   'HasPage'),
      array('Summary',    		'Summary'),
      array('Outputs',  	'Outputs'),
      array('GeoCoverage',    		'GeoCoverage'),
	  array('StartsYearMonth',    		'StartsYearMonth'),
	  array('EndsYearMonth',    'EndsYearMonth'),
      array('People',    		'People'),
      array('WebURL',    		'WebURL'),
      array('Keywords',    		'Keywords'),
      array('Problems',    		'Problems'),
      array('InternalNotes',    'InternalNotes'),
	  array('rel_link_url',    'Related link'),
	  array('rel_link_title',    'Related link title'),
    );

    $csv_file = drupal_get_path('module', 'iied_migrate').'/initia.csv';
    $this->source = new MigrateSourceCSV($csv_file, $columns);

    $this->destination = new MigrateDestinationNode('intiative', array('text_format' => 'full_html'));

    $this->map = new MigrateSQLMap(
      $this->machineName,
      array
      (
        'id' => array
        (
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('is_new')
         ->defaultValue(TRUE);

    $this->addFieldMapping('uid', 'author')
         ->defaultValue(1);

    $this->addFieldMapping('title', 'title');
	$this->addFieldMapping('field_idnum', 'idnum');
	$this->addFieldMapping('field_imp_org', 'ImpOrgan')
		->separator('|');
	$this->addFieldMapping('field_has_page', 'HasPage')
		->separator('|');
	$this->addFieldMapping('field_summary', 'Summary')
		->separator('|');
	$this->addFieldMapping('field_outputs', 'Outputs');
	$this->addFieldMapping('field_geo_cover', 'GeoCoverage')
		->separator('|');
	$this->addFieldMapping('field_start', 'StartsYearMonth');
	$this->addFieldMapping('field_end', 'EndsYearMonth');
	$this->addFieldMapping('field_people', 'People');
    $this->addFieldMapping('field_weburl', 'WebURL');
	$this->addFieldMapping('field_problems', 'Problems');
	$this->addFieldMapping('field_internal_notes', 'InternalNotes');		
	
	
	$this->addFieldMapping('field_rel_link', 'rel_link_url')
		->separator('|')
		->description(t('Mapping related links'));
		
		
	$this->addFieldMapping('pathauto')->defaultValue(TRUE);
$this->addFieldMapping('status')->defaultValue(TRUE);


  }

	public function prepareRow($current_row) {
		
		 	//this replaces a last 0 monthvalue with a 1
		if($current_row->StartsYearMonth){
				$replacement = "1";

		    	$start_date = $current_row->StartsYearMonth;
				$length = strlen($start_date);
				
			//	echo $start_date;

			//echo $length;

			if($length == 1){
				$start_date = '200001';
			}

				if(substr($start_date, -1) == 0){
					$start_date = substr($start_date, 0, -1).$replacement; 
				};
				//echo $article_date;


			$parts = strptime( $start_date, "%Y%m");

			$year = $parts['tm_year'] + 1900; // 1895

		//	echo $year;
			$month = $parts['tm_mon'] + 1; // 10

			$current_row->StartsYearMonth = $year."-".$month;
		} //end if 1
		
		
		// HAndler for end date
		if($current_row->EndsYearMonth){
					$replacement = "1";

			    	$end_date = $current_row->EndsYearMonth;
					$length = strlen($end_date);

				//	echo $length;
					//echo $end_date;

				//echo $length;

				if($length == 1){
					$end_date = '200001';
				}

					if(substr($end_date, -1) == 0){
						$end_date = substr($end_date, 0, -1).$replacement; 
					};
					//echo $article_date;


				$parts = strptime( $end_date, "%Y%m");

				$year = $parts['tm_year'] + 1900; // 1895

			//	echo $year;
				$month = $parts['tm_mon'] + 1; // 10

				$current_row->EndsYearMonth = $year."-".$month;
			} //end if 1

		  }
    

		public function prepare($current_row, $row) 
		{   
			 if (!isset($current_row->field_rel_link['und'][0]['title'])) 
			    {
					$result = $row->rel_link_title;
					$result = explode('|', $result);
					foreach ($current_row->field_rel_link['und'] as $key => $item)
					{
						$current_row->rel_link_title = substr($current_row->rel_link_title, 2);
						$current_row->field_rel_link['und'][$key]['title'] = $result[$key];
					}    
				}


			//print_r($current_row->field_rel_link);

			//	$URL = $current_row->field_rel_link['und'][$key]['url'];
				if (isset($current_row->field_rel_link['und'][0]['url'])) 
				    {
				//	echo '1';
						$URL = $row->rel_link_url;
					//	echo $URL;
						$URL = explode('|', $URL);
					//	print_r($URL);
						foreach ($current_row->field_rel_link['und'] as $key => $item)
						{
					//	echo $URL[$key].'\n';
						//	$current_row->rel_link_url = substr($URL[$key], 0, 1);

					//	echo $current_row->rel_link_url;
						//	$current_row->field_rel_link['und'][$key]['url'] = $URL[$key];
							switch(substr($URL[$key], 0, 1)){
								case 'O':
									echo 'org';
								//	$current_row->rel_link_url = 'en/org/'.$current_row->rel_link_url;
									$current_row->field_rel_link['und'][$key]['url'] = 'en/org/'.	$current_row->field_rel_link['und'][$key]['url'];
									echo 'org DONE';
								break;
									case 'B':
									echo 'bib';
										$current_row->field_rel_link['und'][$key]['url'] = 'en/bib/'.	$current_row->field_rel_link['und'][$key]['url'];
									break;
								case 'D':
									echo 'Doc';
									$current_row->field_rel_link['und'][$key]['url'] = 'en/doc/'.$current_row->field_rel_link['und'][$key]['url'];
									break;
								case 'C':
									echo 'Doc';
									$current_row->field_rel_link['und'][$key]['url'] = 'en/case/'.$current_row->field_rel_link['und'][$key]['url'];
									break;
								case 'E':
									echo 'Event';
									$current_row->field_rel_link['und'][$key]['url'] = 'en/event/'.$current_row->field_rel_link['und'][$key]['url'];
									break;
								case 'N':
									echo '';
									$current_row->field_rel_link['und'][$key]['url'] = '';
									break;	
								case '0':
									echo '';
									$current_row->field_rel_link['und'][$key]['url'] = '';
									break;
							}					
						}

					}			

	  	}
  
}



class LinkMigration extends IiedMigration {
	
	
	//SQL
	//UPDATE `Organ` SET `OrgInterest` = SUBSTR(`OrgInterest`, 2)
	//UPDATE `Organ` SET `GeoCoverage` = SUBSTR(`GeoCoverage`, 2)
	
	
  public function __construct() {
    parent::__construct();

    $this->description = t('Migration of a link');

    $columns = array
    (
      array('id',          	'Unique identifier'),
	  array('idnum',       	'OLD bespoke ID'),
      array('title',    	'Title'),
	  array('Section',    	'section'),
      array('summary',    	'Summary'),
      array('WebURL',    	'WebURL'),
      array('Keywords',    	'Keywords'),
      array('InternalNotes','InternalNotes'),
    );

    $csv_file = drupal_get_path('module', 'iied_migrate').'/link.csv';
    $this->source = new MigrateSourceCSV($csv_file, $columns);

    $this->destination = new MigrateDestinationNode('link', array('text_format' => 'full_html'));

    $this->map = new MigrateSQLMap(
      $this->machineName,
      array
      (
        'id' => array
        (
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('is_new')
         ->defaultValue(TRUE);

    $this->addFieldMapping('uid', 'author')
         ->defaultValue(1);

    $this->addFieldMapping('title', 'title');
	$this->addFieldMapping('field_idnum', 'idnum');
	$this->addFieldMapping('field_section', 'Section');
	$this->addFieldMapping('field_summary', 'summary');
    $this->addFieldMapping('field_weburl', 'WebURL');
	$this->addFieldMapping('field_keywords', 'Keywords');
	$this->addFieldMapping('field_internal_notes', 'InternalNotes');		
		
	$this->addFieldMapping('pathauto')->defaultValue(TRUE);
	$this->addFieldMapping('status')->defaultValue(TRUE);


  }

	public function prepareRow($current_row) {
		
		//prepare func

  }
    
  
}



class DocMigration extends IiedMigration {
	
	
	//SQL
	//UPDATE `Organ` SET `OrgInterest` = SUBSTR(`OrgInterest`, 2)
	//UPDATE `Organ` SET `GeoCoverage` = SUBSTR(`GeoCoverage`, 2)
	
	
  public function __construct() {
    parent::__construct();

    $this->description = t('Migration of a document');

    $columns = array
    (
      array('id',          		'Unique identifier'),
	  array('idnum',       		'OLD bespoke ID'),
      array('DocDate',    		'Document Date'),
	  array('DocTitle',    		'Document Title'),
      array('DocSubtitle',    	'Document Subtitle'),
      array('DocEdition',    	'Document Edition'),
      array('DocLanguage',    	'Document Language'),
      array('DocKilobytes',    	'Document Size'),
      array('DocMedia',    		'Document Media'),
      array('DocFilename',    	'Document Filename'),
      array('DocSelector',    	'Document Selector'),
      array('DocOnServer',    	'Document on server'),
      array('DocKeywords',    	'Document Keywords'),
      array('problems',    		'Problems'),
      array('InternalNotes',	'InternalNotes'),
	  array('rel_link_url',    'Related link'),
	  array('rel_link_title',    'Related link title'),
    );

    $csv_file = drupal_get_path('module', 'iied_migrate').'/document.csv';
    $this->source = new MigrateSourceCSV($csv_file, $columns);

    $this->destination = new MigrateDestinationNode('document', array('text_format' => 'full_html'));

    $this->map = new MigrateSQLMap(
      $this->machineName,
      array
      (
        'id' => array
        (
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('is_new')
         ->defaultValue(TRUE);

    $this->addFieldMapping('uid', 'author')
         ->defaultValue(1);

    $this->addFieldMapping('title', 'DocTitle');
	$this->addFieldMapping('field_idnum', 'idnum');
	$this->addFieldMapping('field_doc_date', 'DocDate');
    $this->addFieldMapping('field_subtitle', 'DocSubtitle');
	$this->addFieldMapping('field_edition', 'DocEdition');
	$this->addFieldMapping('field_doc_language', 'DocLanguage');		
	$this->addFieldMapping('field_media_type', 'DocMedia');		
	$this->addFieldMapping('field_filename', 'DocFilename');		
	$this->addFieldMapping('field_selector_type', 'DocSelector');		
	$this->addFieldMapping('field_problems', 'problems');		
	$this->addFieldMapping('field_keywords', 'DocKeywords');		
	$this->addFieldMapping('field_internal_notes', 'InternalNotes');	
	
	$arguments = MigrateFileFieldHandler::arguments(drupal_get_path('module', 'iied_migrate').'/docs', 'file_copy', FILE_EXISTS_RENAME);
    	
	$this->addFieldMapping('field_document', 'DocFilename')
        ->arguments($arguments);		

	
	$this->addFieldMapping('field_rel_link', 'rel_link_url')
		->separator('|')
		->description(t('Mapping related links'));
		
		
	$this->addFieldMapping('pathauto')->defaultValue(TRUE);
	$this->addFieldMapping('status')->defaultValue(TRUE);


  }

	public function prepareRow($current_row) {
		
		//prepare func

  }

	public function prepare($current_row, $row) 
	{   
		 if (!isset($current_row->field_rel_link['und'][0]['title'])) 
		    {
				$result = $row->rel_link_title;
				$result = explode('|', $result);
				foreach ($current_row->field_rel_link['und'] as $key => $item)
				{
					$current_row->rel_link_title = substr($current_row->rel_link_title, 2);
					$current_row->field_rel_link['und'][$key]['title'] = $result[$key];
				}    
			}
		
		
		//print_r($current_row->field_rel_link);
			
		//	$URL = $current_row->field_rel_link['und'][$key]['url'];
			if (isset($current_row->field_rel_link['und'][0]['url'])) 
			    {
			//	echo '1';
					$URL = $row->rel_link_url;
				//	echo $URL;
					$URL = explode('|', $URL);
				//	print_r($URL);
					foreach ($current_row->field_rel_link['und'] as $key => $item)
					{
				//	echo $URL[$key].'\n';
					//	$current_row->rel_link_url = substr($URL[$key], 0, 1);
				
				//	echo $current_row->rel_link_url;
					//	$current_row->field_rel_link['und'][$key]['url'] = $URL[$key];
						switch(substr($URL[$key], 0, 1)){
							case 'O':
								echo 'org';
							//	$current_row->rel_link_url = 'en/org/'.$current_row->rel_link_url;
								$current_row->field_rel_link['und'][$key]['url'] = 'en/org/'.	$current_row->field_rel_link['und'][$key]['url'];
								echo 'org DONE';
							break;
								case 'B':
								echo 'bib';
									$current_row->field_rel_link['und'][$key]['url'] = 'en/bib/'.	$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'D':
								echo 'Doc';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/doc/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'C':
								echo 'Doc';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/case/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'E':
								echo 'Event';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/event/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'N':
								echo '';
								$current_row->field_rel_link['und'][$key]['url'] = '';
								break;	
							case '0':
								echo '';
								$current_row->field_rel_link['und'][$key]['url'] = '';
								break;
						}					
					}
						    
				}			
		
	}
    
  
}



class EventMigration extends IiedMigration {
	
	
	//SQL
	//UPDATE `Organ` SET `OrgInterest` = SUBSTR(`OrgInterest`, 2)
	//UPDATE `Organ` SET `GeoCoverage` = SUBSTR(`GeoCoverage`, 2)
	
	
  public function __construct() {
    parent::__construct();

    $this->description = t('Migration of a event');

    $columns = array
    (
      array('id',          		'Unique identifier'),
	  array('idnum',       		'OLD bespoke ID'),
      array('EventType',    		'Event Type'),
	  array('EventDate',    		'Event Date'),
      array('EventTitle',    	'Event Title'),
      array('EventSubtitle',    	'Event Subtitle'),
      array('EventSummary',    	'Event summary'),
      array('EventLanguage',    	'Event Language'),
      array('DocCode',    		'Document Code'),
      array('DocSelector',    	'Document Selector'),
      array('EventHyper',    	'Event hyper'),
      array('EventKeywords',    	'Event  keywords'),
      array('problems',    		'Problems'),
      array('InternalNotes',	'InternalNotes'),
	  array('rel_link_url',    'Related link'),
	  array('rel_link_title',    'Related link title'),
    );

    $csv_file = drupal_get_path('module', 'iied_migrate').'/event.csv';
    $this->source = new MigrateSourceCSV($csv_file, $columns);

    $this->destination = new MigrateDestinationNode('event', array('text_format' => 'full_html'));

    $this->map = new MigrateSQLMap(
      $this->machineName,
      array
      (
        'id' => array
        (
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('is_new')
         ->defaultValue(TRUE);

    $this->addFieldMapping('uid', 'author')
         ->defaultValue(1);

    $this->addFieldMapping('title', 'EventTitle');
	$this->addFieldMapping('field_idnum', 'idnum');
	$this->addFieldMapping('field_evt_act_type', 'EventType');
    $this->addFieldMapping('field_sort_date', 'EventDate');
	$this->addFieldMapping('field_subtitle', 'EventSubtitle');		
	$this->addFieldMapping('field_summary', 'EventSummary');		
	$this->addFieldMapping('field_evt_lang', 'EventLanguage');		
	$this->addFieldMapping('field_doc_code', 'DocCode');		
	$this->addFieldMapping('field_doc_select', 'DocSelector');		
	$this->addFieldMapping('field_weburl', 'EventHyper');		
	$this->addFieldMapping('field_keywords', 'EventKeywords');		
	$this->addFieldMapping('field_problems', 'problems');		
	$this->addFieldMapping('field_internal_notes', 'InternalNotes');			
	
	
	$this->addFieldMapping('field_rel_link', 'rel_link_url')
		->separator('|')
		->description(t('Mapping related links'));
		
		
	$this->addFieldMapping('pathauto')->defaultValue(TRUE);
	$this->addFieldMapping('status')->defaultValue(TRUE);


  }

	public function prepareRow($current_row) {
		
		//prepare func

  }
  
  
	public function prepare($current_row, $row) 
	{   
		 if (!isset($current_row->field_rel_link['und'][0]['title'])) 
		    {
				$result = $row->rel_link_title;
				$result = explode('|', $result);
				foreach ($current_row->field_rel_link['und'] as $key => $item)
				{
					$current_row->rel_link_title = substr($current_row->rel_link_title, 2);
					$current_row->field_rel_link['und'][$key]['title'] = $result[$key];
				}    
			}
		
		
		//print_r($current_row->field_rel_link);
			
		//	$URL = $current_row->field_rel_link['und'][$key]['url'];
			if (isset($current_row->field_rel_link['und'][0]['url'])) 
			    {
			//	echo '1';
					$URL = $row->rel_link_url;
				//	echo $URL;
					$URL = explode('|', $URL);
				//	print_r($URL);
					foreach ($current_row->field_rel_link['und'] as $key => $item)
					{
				//	echo $URL[$key].'\n';
					//	$current_row->rel_link_url = substr($URL[$key], 0, 1);
				
				//	echo $current_row->rel_link_url;
					//	$current_row->field_rel_link['und'][$key]['url'] = $URL[$key];
						switch(substr($URL[$key], 0, 1)){
							case 'O':
								echo 'org';
							//	$current_row->rel_link_url = 'en/org/'.$current_row->rel_link_url;
								$current_row->field_rel_link['und'][$key]['url'] = 'en/org/'.	$current_row->field_rel_link['und'][$key]['url'];
								echo 'org DONE';
							break;
								case 'B':
								echo 'bib';
									$current_row->field_rel_link['und'][$key]['url'] = 'en/bib/'.	$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'D':
								echo 'Doc';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/doc/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'C':
								echo 'Doc';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/case/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'E':
								echo 'Event';
								$current_row->field_rel_link['und'][$key]['url'] = 'en/event/'.$current_row->field_rel_link['und'][$key]['url'];
								break;
							case 'N':
								echo '';
								$current_row->field_rel_link['und'][$key]['url'] = '';
								break;	
							case '0':
								echo '';
								$current_row->field_rel_link['und'][$key]['url'] = '';
								break;
						}					
					}
						    
				}			
		
	}
  
}

