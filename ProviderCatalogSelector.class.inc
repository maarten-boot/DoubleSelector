<?php

require_once dirname(__FILE__) . '/DoubleSelector.class.inc';

Class ProviderCatalogSelector Extends DoubleSelector {

	public function __construct($instance)
	{
		parent::__construct($instance);
	}

	public function runSelector()
	{
		$state = parent::runSelector();

		$x_content_s = $this -> getHtmlString();
		$this -> htmlString = $this -> renderHtmlAsForm($x_content_s);

		return $state;
	}
}
