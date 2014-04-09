<?php
if (!class_exists('TOL_secondPass')){
  class TOL_secondPass{
    function set($uid,$pass,$forceEncode=false){
      $uid=$this->_validUid($uid);
      if (empty($pass)){
        throw new Exception('pass is empty',97);
      }
      if (strlen($pass)!=55 || @$pass[0] != '$' || @$pass[2] != '$' || $forceEncode) {
        include "includes/password.inc";
        $pass = _password_crypt('sha512', $pass, _password_generate_salt(DRUPAL_HASH_COUNT));
        //SALT=$S$DIbnEOdVT PASS=$S$DIbnEOdVTPCrW0vLZvlcfVlrelu.K.UzCIfl7fwe.Ep2Psr0LRWq
        //$S$DIbnEOdVT Salt Header
        //$S$   Header ID
        //   D  Posición de iteración strpos('D','./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz');// =15;
        //    IbnEOdVT SALT (RANDOM)
        //
       }
      $account=user_load($uid);
      if ($account){
        $account=new stdClass();
        $account->uid=$uid;
//      unset ($account->pass);
        $edit=array();
        $edit['field_segunda_clave']['und'][0]['value']=$pass;
        $u = user_save($account,$edit);
          return @$u->field_segunda_clave['und'][0]['value'];
      }
      throw new Exception('uid is not valid',98);
    }

    function get($uid){
      $uid=$this->_validUid($uid);
      $account=user_load($uid);
      if ($account){
        return $account->field_segunda_clave['und'][0]['value'];
      }
      return false;
    }

    function validate($uid,$pass,$forceEncode=false){
      $savedPass=$this->get($uid);
      if (strlen($pass)!=55 || @$pass[0] != '$' || @$pass[2] != '$' || $forceEncode) {
        include "includes/password.inc";
        $pass = _password_crypt('sha512', $pass, $savedPass);

      }
      return ($pass==$savedPass);
    }

    private function _validUid($uid){
      if (empty($uid)){
        throw new Exception('uid is empty',97);
      }
      if (!is_numeric($uid)){ //May be is a Gigya UID
        module_load_include('inc','gigya','GigyaUser');
        $GigyaUser=new GigyaUser($uid);
        $uid=$GigyaUser->getDrupalUID();
        if (empty($uid)){
          throw new Exception('uid is not valid',98);
        }
      }
      return $uid;
    }
  }
}
