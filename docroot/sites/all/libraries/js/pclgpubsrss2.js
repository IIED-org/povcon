/* 
 *Pull in RSS feed for author and display it
 */
 
  google.load("feeds", "1");

    function feedLoaded(result) {
      if (!result.error && result.feed.entries.length > 0) {
        var container = document.getElementById("pubs");
        for (var i = 0; i < result.feed.entries.length; i++) {
          var entry = result.feed.entries[i];
          var xml = result.feed.xmldocument;
          var div = document.createElement("div");
          var a = document.createElement("a");
          var img = document.createElement("img");
          var span = document.createElement("span");
          img.setAttribute("src", entry.mediaGroups[0].contents[0].url);
          a.appendChild(img);
          a.appendChild(document.createTextNode(entry.title));
          span.appendChild(document.createTextNode(entry.contentSnippet));
          a.appendChild(span);
          a.setAttribute("href",entry.link);
          div.appendChild(a);
          div.setAttribute("class","pubitem");
          container.appendChild(div);
        }
      }
    }
    
    function OnLoad() {
      var feed = new google.feeds.Feed("http://pubs.iied.org/searchrss2.php?k=This+publication+is+an+output+of+the+Poverty+and+Conservation+Learning+Group+%28PCLG%29");
      feed.setNumEntries(25);
      feed.load(feedLoaded);
    }
    
    google.setOnLoadCallback(OnLoad);