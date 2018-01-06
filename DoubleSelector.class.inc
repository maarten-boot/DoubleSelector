<?php

Class DoubleSelector {

	///////////////////////////////
	///////////////////////////////
	public $dbg;

	protected $initialized;

	protected $instance;

	protected $size = 10;
	protected $noCommit = FALSE;

	protected $htmlString = NULL;

	protected $sessionData = array(
		'available' => NULL,
		'assigned' => NULL,
		'title' => NULL,
	);

	protected $postData = array(
		'posted' => NULL,
		'title' => NULL,

		'Assign' => NULL,
		'Return' => NULL,
		'Commit' => NULL,

		'left' => NULL,		// the currently selected data on the left
		'right' => NULL,	// the current selected data on the right
	);

	///////////////////////////////
	///////////////////////////////

	protected function mkListLeft()
	{
		$x_avail = array();
		foreach($this -> sessionData['available'] as $key => $val) {
			if( in_array($key,array_keys($this -> sessionData['assigned'])) ) {
				continue;
			}
			$x_avail[$key] = $val;
		}

		$x_len = count($x_avail);
		$y_len = count($this -> sessionData['assigned']);

		$this -> size = max($x_len,$y_len);

		return $x_avail;
	}

	protected function getSessionData()
	{
		$instance = $this -> instance;

		if( ! isset($_SESSION[__CLASS__][$instance]) ) {
			return;
		}

		$x_fields = array_keys($this -> sessionData);

		foreach($x_fields as $f) {
			$this -> sessionData[$f] = NULL;
			if( isset( $_SESSION[__CLASS__][$instance][$f] )) {
				$this -> sessionData[$f] = $_SESSION[__CLASS__][$instance][$f];
			}
		}
	}

	protected function setSessionData()
	{
		$instance = $this -> instance;

		$x_fields = array_keys($this -> sessionData);
		$_SESSION[__CLASS__][$instance] = array();
		foreach($x_fields as $f) {
			$_SESSION[__CLASS__][$instance][$f] = NULL;
			if(isset($this -> sessionData[$f]) ) {
				$_SESSION[__CLASS__][$instance][$f] = $this -> sessionData[$f];
			}
		}
	}

	protected function getPostData()
	{
		$instance = $this -> instance;
		$class = __CLASS__;
		if( ! isset($_POST[$class][$instance]['posted'] ) ) {
			return;
		}

		foreach($this -> postData as $key => $dummy) {
			$this -> postData[$key] = NULL;
			if( isset($_POST[$class][$instance][$key]) ) {
				$this -> postData[$key] = $_POST[$class][$instance][$key];
			}
		}
	}

	protected function processPostData()
	{
		// if you pressed -->, take all selected elements from the left and add them to assigned, then you are done
		// Assign
		if( isset($this -> postData['Assign'])) {
			foreach($this -> postData['left'] as $index => $key) {
				foreach($this -> sessionData['available'] as $aKey => $aVal) {
					if( $key === $aKey) {
						$this -> sessionData['assigned'][$aKey] = $aVal;
					}
				}
			}

			asort($this -> sessionData['assigned']);

			return FALSE;	// indicate we are not submitting yet
		}

		// if you pressed <--, take all selected elements from the right and remove them from assigned, then you are done
		// Return
		if( isset($this -> postData['Return'])) {
			$zz = array();
			$yy = array_values($this -> postData['right']);
			foreach($this -> sessionData['assigned'] as $aKey => $aVal) {
				if(in_array($aKey,$yy)) {
					continue;
				}
				$zz[$aKey] = $aVal;
			}

			asort($zz);

			// set the nest data
			$this -> sessionData['assigned'] = $zz;

			return FALSE;	// indicate we are not submitting yet
		}

		// if you press commit, then you just set the finisged flag, the current assigned stays unchanged, then you are done
		// Commit
		if( isset($this -> postData['Commit'])) {

			return TRUE;	// indicate we are submitting
		}
	}

	////////////////////
	/// VIEW

	protected function renderHtmlBasicMultiSelect($label,$data)
	{
		$x_a = array();

		foreach($data as $key => $val) {
			$selected = '';

			$x_a[] = <<<EOS
	<option value="$key" $selected > $val </option>
EOS;
		}
		$x_options_s = implode("\n", $x_a);

		$class = __CLASS__;
		$instance = $this -> instance;
		$size = $this -> size;

		$x_s = <<<EOS
<select style="width: 100%;" name="{$class}[{$instance}][{$label}][]" multiple="multiple" size="{$size}">
$x_options_s
</select>
EOS;

		return $x_s;
	}

	protected function renderHtmlSelector()
	{
		$x_a = array();

		$x_avail = $this -> mkListLeft();
		$label = 'left';
		$avail_select = $this -> renderHtmlBasicMultiSelect($label,$x_avail);

		$label = 'right';
		$assign_select = $this -> renderHtmlBasicMultiSelect($label,$this -> sessionData['assigned']);

		$class = __CLASS__;
		$instance = $this -> instance;
		$title = $this -> sessionData['title'];
		if( $title ) {
			$x_title = <<<EOS
		<tr >
			<td colspan="3">
				<div style="text-align: center; margin: 3px 5px;">
					Title: {$title}
				</div>
			</td>
		</tr>
EOS;
		}

		$x_commit = '';
		if( $this -> noCommit === FALSE ) {
			$x_commit = <<<EOS
		<tr >
			<td colspan="3">
				<div style="text-align: center; margin: 3px 5px;">
					<input type="submit" name="{$class}[{$instance}][Commit]" value="x_commit" />
				</div>
			</td>
		</tr>
EOS;
		}

		$x_a[] = <<<EOS
<div style="border: 1px solid black;">
	<input type="hidden" name="{$class}[{$instance}][posted]" value="1" />
	<input type="hidden" name="{$class}[{$instance}][title]" value="{$title}" />

	<table border="0" style="border-collapse:collapse;" >
		$x_title
		<tr>
			<td>
				<div style="text-align: center; margin: 3px 5px;">Available</div>
				$avail_select
			</td>
			<td>
				<table>
					<tr>
						<td>
							<div style="text-align: center;">
								<input type="submit" name="{$class}[{$instance}][Assign]" value="--->" />
							</div>
						</td>
					</tr>
					<tr>
						<td>
						</td>
					</tr>
					<tr>
						<td>
							<div style="text-align: center;">
								<input type="submit" name="{$class}[{$instance}][Return]" value="<---" />
							</div>
						</td>
					</tr>
				</table>
			</td>
			<td>
				<div style="text-align: center; margin: 3px 5px;">Selected</div>
				$assign_select
			</td>
		</tr>
		$x_commit
	</table>
</div>
EOS;
		return implode("\n", $x_a);
	}

	protected function renderHtmlAsForm($x_content_s)
	{
		$me = $_SERVER['PHP_SELF'];

		$x_s = <<<EOS
<form name="DoubleSelector" method="POST" action="$me">
		$x_content_s
</form>
EOS;

		return $x_s;
	}

	/////////////////////////////
	// Public

	public function __construct($instance)
	{
		session_start();

		$this -> instance = $instance;
		$this -> initialized = FALSE;

		if( isset($_SESSION[__CLASS__][$instance] ) ) {
			$this -> getSessionData();
			$this -> initialized = TRUE;
		}
	}

	public function __set($name,$val)
	{
		$sessionData_keys = array_keys($this -> sessionData);

		if( ! $this -> initialized ) {
			if( in_array($name,$sessionData_keys)) {
				$this -> sessionData[$name] = $val;
			}
		}
		// ignore all unknown field names silently
	}

	public function setNoCommit()
	{
		$this -> noCommit = TRUE;
	}

	public function getHtmlString()
	{
		return $this -> htmlString;
	}

	public function getAssigned()
	{
		$data = $this -> sessionData['assigned'];
		$this -> resetState();
		return $data;
	}

	public function debugSessionData()
	{
		if($this -> dbg) {
			print '<pre>';
			print 'POST: ';
			print print_r($_POST,TRUE);
			print '<br/>SESSION: ';

			print print_r($_SESSION,TRUE);
			print '</pre>';
		}
	}

	public function debugPostData()
	{
		if($this -> dbg) {
			print '<pre>';
			print 'postData';
			print print_r($this->postData,TRUE);
			print '<br/>sessionData';
			print print_r($this -> sessionData,TRUE);
			print '</pre>';
		}
	}

	public function resetState()
	{
		$instance = $this -> instance;
		$x_fields = array_keys($this -> sessionData);
		foreach($x_fields as $f) {
			// $_SESSION[__CLASS__][$instance][$f] = NULL;
			$this -> sessionData[$f] = NULL;
		}
		$_SESSION[__CLASS__][$instance] = NULL;
		unset($_SESSION[__CLASS__][$instance]);

		// $this -> setSessionData();
	}

	public function runSelector()
	{
		// $this -> getSessionData();
		$this -> getPostData();

		$state = $this -> processPostData();
		if( $state ) {
			return TRUE;
		}

		$this -> htmlString = $this -> renderHtmlSelector();

		//$x_content_s = $this -> renderHtmlSelector();
		//$this -> htmlString = $this -> renderHtmlAsForm($x_content_s);

		$this -> debugPostData();
		$this -> setSessionData();
		$this -> debugSessionData();

		return FALSE;
	}
}
