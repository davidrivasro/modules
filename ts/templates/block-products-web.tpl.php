<div class="list-features">
  <ul>
  <?php foreach($features as $feature):?>
    <li><?php print $feature->description; ?></li>
  <?php endforeach; ?>
  </ul>
</div>
<div class="products-web">
<?php foreach($products as $product): ?>
<div class="row">
  <div class="header">
    <h1><?php print $product->title; ?></h1>
    <h2><?php print $product->title_reference1; ?></h2>
    <h3><?php print $product->title_reference2; ?></h3>
  </div>
  <div class="price"><?php print $product->price_prefix; ?><?php print $product->price; ?>/<?php print $product->price_period; ?></div>
  <?php if(!empty($product->features )): ?>
  <div class="properties">
    <ul>
    <?php foreach($product->features as $item):?>
      <li><?php print $item; ?></li>
    <?php endforeach; ?>
    </ul>
  </div>
<?php endif; ?>
</div>
<?php endforeach; ?>
</div>
