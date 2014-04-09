<?php
if (!class_exists('TOL_Lines')){
  class TOL_Lines{

    function get($uid) {
      if (empty($uid)){
      	throw new Exception('uid is empty',97);
      }

      if (!is_numeric($uid)){ //May be is a Gigya UID
        module_load_include('inc','gigya','GigyaUser');
        $GigyaUser=new GigyaUser($uid);
      
        $uid=$GigyaUser->getDrupalUID();
        if (empty($uid)){ //No existe el usuario en bbdd
          $gigyaUserInfo= $GigyaUser->getGCSUserData();
      
          if (!empty($gigyaUserInfo['data']['name_s'])){
            $account=gigya_create_drupal_user($gigyaUserInfo['data']);
            $uid=$account->uid;
          }else{ //no data en GCS
            throw new Exception('invalid Gigya User, GCS Data name_s missing',97);
          }
        }
      }
      if (empty($uid)){
        throw new Exception('user not exists',97);
      }

      $lines = &drupal_static('TOL_Lines.get_'.$uid);
      
      if (!isset($lines)){
        $lines =  array();
        $query = new EntityFieldQuery();
        $result = $query->entityCondition('entity_type', 'lineas_celulares_usuario')
          ->entityCondition('bundle', 'lineas_celulares_usuario')
          ->propertyCondition('uid', $uid)
          ->execute();
        if ($result) {
          $ids=array();
          foreach ($result['lineas_celulares_usuario'] as $key => $value) {
            $ids[] = $value->id;
          }
          $result = entity_load('lineas_celulares_usuario', $ids);
          if (!empty($result)){
            $lines=array();
            foreach ($result as $key => $value) {
              $obj=(object)array(
                'id' => $value->id,
                'uid' => $value->uid,
                'name'=>null,
                'line' => @$value->field_linea[LANGUAGE_NONE][0]['value'],
                'servicio_buzon' => @$value->field_servicio_buzon[LANGUAGE_NONE][0]['value'],
                'servicio_sms_promocionales' => @$value->field_servicio_sms_promocionales[LANGUAGE_NONE][0]['value'],
                'sms' => @$value->field_sms[LANGUAGE_NONE][0]['value'],
                'notificacion_factura' => @$value->field_activar_notificacion_factu[LANGUAGE_NONE][0]['value'],

              );
              if (!empty($value->field_linea_nombre[LANGUAGE_NONE][0]['value'])){
                $obj->name=$value->field_linea_nombre[LANGUAGE_NONE][0]['value'];
              }
              $lines[]=$obj;
            }
          }
        }
      }
      return $lines;
    }


    function del($uid,$msisdn) {
      if (!is_numeric($msisdn)){
        throw new Exception('Wrong msisdn format',98);
      }
      $lines=$this->get($uid);
      foreach($lines as $line){
        if ($line->line==$msisdn){
          return entity_delete('lineas_celulares_usuario', $line->id);
        }
      }
      throw new Exception('line not exists',98);
    }

    function delete_all($uid){
      $query = new EntityFieldQuery();
      $result = $query->entityCondition('entity_type', 'lineas_celulares_usuario')
        ->entityCondition('bundle', 'lineas_celulares_usuario')
        ->propertyCondition('uid', $uid)
        ->execute();

      if (!empty($result)) {
        $ids=array();
        foreach ($result['lineas_celulares_usuario'] as $key => $value) {
          $ids[] = $value->id;
        }
        entity_delete('lineas_celulares_usuario', $ids);
      }

    }

    function add($uid,$msisdn,$sms=null,$name=null){
      if (empty($uid)){
        throw new Exception('uid is empty',97);
      }elseif (!is_numeric($msisdn)){
        throw new Exception('Wrong msisdn format',98);
      }
      if (!is_numeric($uid)){ //May be is a Gigya UID
        module_load_include('inc','gigya','GigyaUser');
        $GigyaUser=new GigyaUser($uid);
        $uid=$GigyaUser->getDrupalUID();
      }

      if (!is_numeric($uid)){
         throw new Exception('user not exists',98);
      }

      $query = new EntityFieldQuery();
      $result = $query->entityCondition('entity_type', 'lineas_celulares_usuario')
          ->entityCondition('bundle', 'lineas_celulares_usuario')
          ->fieldCondition('field_linea', 'value', $msisdn, '=', 0)
          ->execute();

      if ($result){
        throw new Exception('Line already exist',99);
      }
      $entity = entity_create('lineas_celulares_usuario', array('type' =>'lineas_celulares_usuario','uid'=>$uid));
      $entity->field_linea = array(LANGUAGE_NONE => array(0 => array('value' => $msisdn)));
      if (!empty($sms)){
        $entity->field_sms = array(LANGUAGE_NONE => array(0 => array('value' => $sms)));
      }
      if (!empty($name)){
        $entity->field_linea_nombre = array(LANGUAGE_NONE => array(0 => array('value' => $name)));
      }
   
      return $entity->save();
    }


    function modifyName($uid,$msisdn,$name){
      if (!is_numeric($uid)){
        throw new Exception('Ivalid uid',97);
      }elseif (!is_numeric($msisdn)){
        throw new Exception('Wrong msisdn format',98);
      }elseif (empty($name)){		  
        //throw new Exception('Empty Name',98);
        drupal_set_message(t('No hemos podido actualizar los datos, debes colocar un nombre a la línea.'), 'error');
        drupal_goto('online/mis-lineas');        
      }

      $query = new EntityFieldQuery();
      $result = $query->entityCondition('entity_type', 'lineas_celulares_usuario')
          ->entityCondition('bundle', 'lineas_celulares_usuario')
          ->propertyCondition('uid', $uid)
          ->fieldCondition('field_linea', 'value', $msisdn, '=', 0)
          ->execute();
      $result=@$result['lineas_celulares_usuario'];
      if (empty($result)){
        throw new Exception('msisdn does not exist',99);
      }
      $result=current($result);
      $lineas=entity_get_controller('lineas_celulares_usuario');
      $linea = $lineas->load(array($result->id));
      $linea=current($linea);
    	$linea->field_linea_nombre['und'][0]['value'] = $name; //modify the name
      return $linea->save();
    }


    function getByMSISDN($linesObj,$msisdn) {
      if (empty($msisdn)){
        throw new Exception('msisdn is empty',97);
      }

      if (empty($linesObj)){
        throw new Exception('linesObj is empty',98);
      }

		  foreach ($linesObj as $key => $value) {
		    if((string)$value->line == $msisdn) {
		      return $value;
		    }
		  }
		  return NULL;
    }

  }
}
