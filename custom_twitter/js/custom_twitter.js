jQuery(document).ready(function($){
    $('#tweetFeed').jTweetsAnywhere({
        username: 'enterco',
        count: 2,
        showTweetFeed: {
            showProfileImages: true,
            showUserScreenNames: false,
            showUserFullNames: false,
            showActionReply: false,
            showActionRetweet: false,
            showActionFavorite: false,
        },
        showTweetBox: {
            label: '<span style="color: #303030">Spread the word ...</span>'
        }
    });
});
