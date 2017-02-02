<div id="page" class="<?php print $classes; ?>"<?php print $attributes; ?>>
	

  <!-- ______________________ HEADER _______________________ -->

  <div id="header">

    <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>"/>
      </a>
    <?php endif; ?>

    <?php if ($site_name || $site_slogan): ?>
      <div id="name-and-slogan">

        <?php if ($site_name): ?>
          <?php if ($title): ?>
            <div id="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
            </div>
          <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 id="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
            </h1>
          <?php endif; ?>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div id="site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>

      </div>
    <?php endif; ?>

    <?php if ($page['header']): ?>
      <div id="header-region">
        <?php print render($page['header']); ?>			
      </div>
    <?php endif; ?>

	<?php if ($page['search']): ?>
      <div id="search-region">
        <?php print render($page['search']); ?>			
      </div>
    <?php endif; ?>

   <?php print $feed_icons; ?>
   


	<div id="main-nav">
		<?php print render($page['main_navigation']); ?>
		<?php if ($main_menu || $secondary_menu): ?>
	    <div id="navigation" class="menu <?php if (!empty($main_menu)) {print "with-primary";} if (!empty($secondary_menu)) {print " with-secondary";} ?>">
	        <?php print theme('links', array('links' => $main_menu, 'attributes' => array('id' => 'primary', 'class' => array('links', 'clearfix', 'main-menu')))); ?>
	        <?php print theme('links', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary', 'class' => array('links', 'clearfix', 'sub-menu')))); ?>
	      </div>
	    <?php endif; ?>
	
	</div><!--close main navigation -->
	 
      <!-- 
	   <div id="for-links">
   	<ul>
   		<li><a href="/en/pages/espanol">Espanol</a></li>
   		<li style="border-right:none"><a href="/en/pages/francais">Francais</a></li>
   	</ul>
   </div> -->

  </div> <!-- /header -->

  <!-- ______________________ MAIN _______________________ -->

  <div id="main" class="clearfix">
	
		<div id="home-slider">
			<?php print render($page['slider']); ?>
		</div>

    <div id="content">
      <div id="content-inner" class="inner column center">

          <div id="content-header">

            <?php print $breadcrumb; ?>


            <?php if ($title): ?>
              <h1 class="title"><?php print $title; ?></h1>
            <?php endif; ?>

            
          </div> <!-- /#content-header -->

	

        <div id="content-area">
          <?php //print render($page['content']) ?>
			
			
			<div id="home-content-1" class="content-box">
				<?php print render($page['content_area_1']); ?>
			</div>
			<div id="home-content-2" class="content-box">
				<?php print render($page['content_area_2']); ?>
        	</div>
		</div>

     

      </div>
    </div> <!-- /content-inner /content -->
    
    <div id="main-sidebar">


	<?php if ($page['editors_pick']): ?>
      <div id="editors-pick" class="column sidebar top">
		<h2>Top resources</h2>
        <div id="editors-pick-inner" class="inner">
          <?php print render($page['editors_pick']); ?>
        </div>
      </div>
    <?php endif; ?> <!-- /latest-updates -->
    
        <?php if ($page['sidebar_first']): ?>
      <div id="sidebar-first" class="column sidebar first">
        <div id="sidebar-first-inner" class="inner">
          <?php print render($page['sidebar_first']); ?>
        </div>
      </div>
    <?php endif; ?> <!-- /sidebar-first -->

	<?php if ($page['latest_updates']): ?>
      <div id="latest-updates" class="column sidebar third">
		<h2>Latest updates</h2>
        <div id="latest-updates-inner" class="inner">
          <?php print render($page['latest_updates']); ?>
        </div>
      </div>
    <?php endif; ?> <!-- /latest-updates -->
    

    <?php if ($page['sidebar_second']): ?>
      <div id="sidebar-second" class="column sidebar second">
        <div id="sidebar-second-inner" class="inner">
          <?php print render($page['sidebar_second']); ?>
        </div>
      </div>
    <?php endif; ?> <!-- /sidebar-second -->

	</div>
	



  </div> <!-- /main -->

 <?php if ($page['sponsors']): ?>
      <div id="sponsors" class="sponsors">
        <div id="sponsors-inner" class="inner">
          <?php print render($page['sponsors']); ?>
        </div>
      </div>
  <?php endif; ?> <!-- /sidebar-first -->


</div> <!-- /page -->

<!-- ______________________ FOOTER _______________________ -->

<?php if ($page['footer_box1']): ?>
  <div id="footer">
  	<div id="footer-inner">
	
	
	<div id="footer-box1">
    	<?php print render($page['footer_box1']); ?>
	</div>
	<div id="footer-box2">
    	<?php print render($page['footer_box2']); ?>
	</div>
	<div id="footer-box3">
    	<?php print render($page['footer_box3']); ?>
	</div>
	<div id="footer-box4">
    	<?php print render($page['footer_box4']); ?>
	</div>
	</div>
  </div> <!-- /footer -->
<?php endif; ?>
