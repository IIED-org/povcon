<div id="block-<?php print $block->module .'-'. $block->delta ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="block-inner">

      <?php print render($title_prefix); ?>
    <?php if ($block->subject): ?>
	<div class="title-left"></div>
      <h2 class="block-title"<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
	<div class="title-right"></div>
	<div style="clear:both;"></div>
    <?php endif;?>
      <?php print render($title_suffix); ?>
		
		<div class="content" <?php print $content_attributes; ?>>
		  <?php print $content; ?>
		</div>

  </div>
</div> <!-- /block-inner /block -->