<?php

function ts_faq_content_faq_page() {
  $faqs = ts_faq_get_faqs();

  $build['ts_faqs'] = array();

  $count = count($faqs);
  if ($count == 1) {
    $faq = array_shift($faqs);
    $build['ts_faqs'] = ts_faq_format_category($faq);
  }
  elseif ($count > 1) {
    $build['ts_faqs'] = array(
      '#type' => 'vertical_tabs',
      );
    foreach ($faqs as $faq) {
      $key = 'fieldset_' . $faq->nid;
      $build['ts_faqs'][$key] = array(
        '#type' => 'fieldset',
        '#title' => $faq->title,
        );
      $build['ts_faqs'][$key]['content'] = ts_faq_format_category($faq);
    }
  }

  return $build;
}

function ts_faq_get_faqs() {
  $faqs = array();

  $query = db_select('node', 'n');
  $query->fields('n', array('nid'));
  $query->condition('type', 'faqs');
  $query->condition('status', 1);

  $result = $query->execute();

  foreach ($result as $row) {
    $faqs[$row->nid] = node_load($row->nid);
  }

  return $faqs;
}

function ts_faq_format_category($faq) {

  $header = array(
    '#type' => 'html_tag',
    '#tag' => 'h2',
    '#value' => '¿Necesitas Ayuda?',
    '#attributes' => array(
      'class' => array(
        'ts-faq-header',
        ),
      ),
    );
  $questions = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#value' => '',
    '#attributes' => array(
      'class' => array(
        'ts-faq-questions',
        ),
      ),
    );

  $answers = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('ts-faq-answers',),
      ),
    );


  foreach ($faq->field_question[LANGUAGE_NONE] as $key => $question) {
    $question = field_collection_item_load($question['value']);
    ++$key;
    $answer_id = 'answer_' . $faq->nid . '_' . $question->item_id;

    $questions['#value'] .= '<li><span class="number">'.$key.'</span>' . l($question->field_title[LANGUAGE_NONE][0]['safe_value'], TS_FAQ_PATH, array('fragment' => $answer_id)) . '</li>';

    $answers[$answer_id] = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => $answer_id,
        'class' => 'answer-row',
        ),
      );

    $answers[$answer_id]['question'] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#value' => '<span>' . $key . '</span> ' . $question->field_title[LANGUAGE_NONE][0]['safe_value'],
      '#attributes' => array(
        'class' => 'question',
        ),
      );

    $answers[$answer_id]['answer'] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#value' => $question->field_answer[LANGUAGE_NONE][0]['safe_value'],
      '#attributes' => array(
        'class' => 'answer',
        ),
      );

    if (!empty($question->field_faq_image[LANGUAGE_NONE][0]['uri'])) {
      $variables = array(
        'path' => $question->field_faq_image[LANGUAGE_NONE][0]['uri'],
        );
      $answers[$answer_id]['image'] = array(
        '#type' => 'html_tag',
        '#tag' => 'div',
        '#value' => theme('image', $variables),
        '#attributes' => array(
          'class' => 'image',
          ),
        );
    }

    $answers[$answer_id]['back'] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#value' => l(t("Back to top"), TS_FAQ_PATH, array('fragment' => 'main')),
      '#attributes' => array(
        'class' => 'back',
        ),
      );

  }

  $questions['#value'] = '<ul>' . $questions['#value'] . '</ul>';

  return array(
    'header' => $header,
    'questions' => $questions,
    'answers' => $answers,
    );
}
