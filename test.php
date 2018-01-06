<?php

require_once dirname(__FILE__) . '/ProviderCatalogSelector.class.inc';

function xx_test()
{
	$title = 'aaaa';

	$available = array(
		'xxx_001' => 'xxx_001',
		'xxx_002' => 'xxx_002',
		'xxx_003' => 'xxx_003',
		'xxx_004' => 'xxx_004',
		'xxx_005' => 'xxx_005',
		'xxx_006' => 'xxx_006',
		'xxx_007' => 'xxx_007',
		'xxx_008' => 'xxx_008',
	);

	$assigned = array(
		'xxx_006' => 'xxx_006',
		'xxx_007' => 'xxx_007',
		'xxx_008' => 'xxx_008',
	);

	$instance = 'xxxyyy';

	$ProviderCatalogSelector = New ProviderCatalogSelector($instance);

	$ProviderCatalogSelector -> title = $title;
	$ProviderCatalogSelector -> available = $available;
	$ProviderCatalogSelector -> assigned = $assigned;

	if( $ProviderCatalogSelector -> runSelector() ) {
		$data = $ProviderCatalogSelector -> getAssigned();
		if(1) {
			print '<pre>';
			print print_r($data,TRUE);
			print '</pre>';
		}
	} else {
		$x_s = $ProviderCatalogSelector -> getHtmlString();
		print <<<EOS
		<div style="float: left;">
			$x_s
		</div>
EOS;
	}

	$instance2 = 'n2';

	// $ProviderCatalogSelector2 = New ProviderCatalogSelector($instance2);
	$ProviderCatalogSelector2 = New DoubleSelector($instance2);

	// $ProviderCatalogSelector2 -> title = 'bbbb';
	$ProviderCatalogSelector2 -> available = $available;
	$ProviderCatalogSelector2 -> assigned = $assigned;
	// $ProviderCatalogSelector2 -> setNoCommit();

	if( $ProviderCatalogSelector2 -> runSelector() ) {
		$data = $ProviderCatalogSelector2 -> getAssigned();
		if(1) {
			print '<pre>';
			print print_r($data,TRUE);
			print '</pre>';
		}
	} else {
		$me = $_SERVER['PHP_SELF'];
		$x_s = $ProviderCatalogSelector2 -> getHtmlString();

		print <<<EOS
		<div style="float: left;">
			<form name="nnnnn" method="POST" action="$me">
				$x_s
			</form>
		</div>
EOS;
	}
}

xx_test();

