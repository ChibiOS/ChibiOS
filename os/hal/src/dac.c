




<!DOCTYPE html>
<html>
  <head prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb# object: http://ogp.me/ns/object# article: http://ogp.me/ns/article# profile: http://ogp.me/ns/profile#">
    <meta charset='utf-8'>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title>ChibiOS/os/hal/src/dac.c at dac-3.0 · mobyfab/ChibiOS · GitHub</title>
    <link rel="search" type="application/opensearchdescription+xml" href="/opensearch.xml" title="GitHub" />
    <link rel="fluid-icon" href="https://github.com/fluidicon.png" title="GitHub" />
    <link rel="apple-touch-icon" sizes="57x57" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="114x114" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="72x72" href="/apple-touch-icon-144.png" />
    <link rel="apple-touch-icon" sizes="144x144" href="/apple-touch-icon-144.png" />
    <meta property="fb:app_id" content="1401488693436528"/>

      <meta content="@github" name="twitter:site" /><meta content="summary" name="twitter:card" /><meta content="mobyfab/ChibiOS" name="twitter:title" /><meta content="Mirror of the ChibiOS SVN repository (updated every 10 minutes)" name="twitter:description" /><meta content="https://1.gravatar.com/avatar/f152cebdb9ab085fd2369e2e9eb7342b?d=https%3A%2F%2Fidenticons.github.com%2Fdecfe48600b86598c97bc3499607b4a9.png&amp;r=x&amp;s=400" name="twitter:image:src" />
<meta content="GitHub" property="og:site_name" /><meta content="object" property="og:type" /><meta content="https://1.gravatar.com/avatar/f152cebdb9ab085fd2369e2e9eb7342b?d=https%3A%2F%2Fidenticons.github.com%2Fdecfe48600b86598c97bc3499607b4a9.png&amp;r=x&amp;s=400" property="og:image" /><meta content="mobyfab/ChibiOS" property="og:title" /><meta content="https://github.com/mobyfab/ChibiOS" property="og:url" /><meta content="Mirror of the ChibiOS SVN repository (updated every 10 minutes)" property="og:description" />

    <meta name="hostname" content="github-fe122-cp1-prd.iad.github.net">
    <meta name="ruby" content="ruby 2.1.0p0-github-tcmalloc (87c9373a41) [x86_64-linux]">
    <link rel="assets" href="https://github.global.ssl.fastly.net/">
    <link rel="conduit-xhr" href="https://ghconduit.com:25035/">
    <link rel="xhr-socket" href="/_sockets" />


    <meta name="msapplication-TileImage" content="/windows-tile.png" />
    <meta name="msapplication-TileColor" content="#ffffff" />
    <meta name="selected-link" value="repo_source" data-pjax-transient />
    <meta content="collector.githubapp.com" name="octolytics-host" /><meta content="collector-cdn.github.com" name="octolytics-script-host" /><meta content="github" name="octolytics-app-id" /><meta content="A4810129:3BDF:E1E919:5316F9AC" name="octolytics-dimension-request_id" />
    

    
    
    <link rel="icon" type="image/x-icon" href="/favicon.ico" />

    <meta content="authenticity_token" name="csrf-param" />
<meta content="MvW7hvzG7lJFM5drAVOhCoRhib1O7GhIJBqEsW88KxU=" name="csrf-token" />

    <link href="https://github.global.ssl.fastly.net/assets/github-144f6fbd93cc11df8cc03bc000351f75fa1e5875.css" media="all" rel="stylesheet" type="text/css" />
    <link href="https://github.global.ssl.fastly.net/assets/github2-ff993fcf465373a314708ecedfbbffc29250b930.css" media="all" rel="stylesheet" type="text/css" />
    
    


      <script crossorigin="anonymous" src="https://github.global.ssl.fastly.net/assets/frameworks-490867a148cf94468c141093ccd4e56ed2144eb9.js" type="text/javascript"></script>
      <script async="async" crossorigin="anonymous" src="https://github.global.ssl.fastly.net/assets/github-a74d8739a439667125da3a5c5435962d57986a54.js" type="text/javascript"></script>
      
      <meta http-equiv="x-pjax-version" content="348cbaf0429fd819da9310a1f5148dfb">

        <link data-pjax-transient rel='permalink' href='/mobyfab/ChibiOS/blob/42c482fbf2b4feae8dd524e664d084bb6de4f9ba/os/hal/src/dac.c'>

  <meta name="description" content="Mirror of the ChibiOS SVN repository (updated every 10 minutes)" />

  <meta content="1305966" name="octolytics-dimension-user_id" /><meta content="mobyfab" name="octolytics-dimension-user_login" /><meta content="10358524" name="octolytics-dimension-repository_id" /><meta content="mobyfab/ChibiOS" name="octolytics-dimension-repository_nwo" /><meta content="true" name="octolytics-dimension-repository_public" /><meta content="true" name="octolytics-dimension-repository_is_fork" /><meta content="3123604" name="octolytics-dimension-repository_parent_id" /><meta content="mabl/ChibiOS" name="octolytics-dimension-repository_parent_nwo" /><meta content="3123604" name="octolytics-dimension-repository_network_root_id" /><meta content="mabl/ChibiOS" name="octolytics-dimension-repository_network_root_nwo" />
  <link href="https://github.com/mobyfab/ChibiOS/commits/dac-3.0.atom" rel="alternate" title="Recent Commits to ChibiOS:dac-3.0" type="application/atom+xml" />

  </head>


  <body class="logged_out  env-production windows vis-public fork page-blob tipsy-tooltips">
    <div class="wrapper">
      
      
      
      


      
      <div class="header header-logged-out">
  <div class="container clearfix">

    <a class="header-logo-wordmark" href="https://github.com/">
      <span class="mega-octicon octicon-logo-github"></span>
    </a>

    <div class="header-actions">
        <a class="button primary" href="/join">Sign up</a>
      <a class="button signin" href="/login?return_to=%2Fmobyfab%2FChibiOS%2Fblob%2Fdac-3.0%2Fos%2Fhal%2Fsrc%2Fdac.c">Sign in</a>
    </div>

    <div class="command-bar js-command-bar  in-repository">

      <ul class="top-nav">
          <li class="explore"><a href="/explore">Explore</a></li>
        <li class="features"><a href="/features">Features</a></li>
          <li class="enterprise"><a href="https://enterprise.github.com/">Enterprise</a></li>
          <li class="blog"><a href="/blog">Blog</a></li>
      </ul>
        <form accept-charset="UTF-8" action="/search" class="command-bar-form" id="top_search_form" method="get">

<input type="text" data-hotkey=" s" name="q" id="js-command-bar-field" placeholder="Search or type a command" tabindex="1" autocapitalize="off"
    
    
      data-repo="mobyfab/ChibiOS"
      data-branch="dac-3.0"
      data-sha="93755360953aeba3601c74f4ac7e05e176c07d0b"
  >

    <input type="hidden" name="nwo" value="mobyfab/ChibiOS" />

    <div class="select-menu js-menu-container js-select-menu search-context-select-menu">
      <span class="minibutton select-menu-button js-menu-target" role="button" aria-haspopup="true">
        <span class="js-select-button">This repository</span>
      </span>

      <div class="select-menu-modal-holder js-menu-content js-navigation-container" aria-hidden="true">
        <div class="select-menu-modal">

          <div class="select-menu-item js-navigation-item js-this-repository-navigation-item selected">
            <span class="select-menu-item-icon octicon octicon-check"></span>
            <input type="radio" class="js-search-this-repository" name="search_target" value="repository" checked="checked" />
            <div class="select-menu-item-text js-select-button-text">This repository</div>
          </div> <!-- /.select-menu-item -->

          <div class="select-menu-item js-navigation-item js-all-repositories-navigation-item">
            <span class="select-menu-item-icon octicon octicon-check"></span>
            <input type="radio" name="search_target" value="global" />
            <div class="select-menu-item-text js-select-button-text">All repositories</div>
          </div> <!-- /.select-menu-item -->

        </div>
      </div>
    </div>

  <span class="help tooltipped tooltipped-s" aria-label="Show command bar help">
    <span class="octicon octicon-question"></span>
  </span>


  <input type="hidden" name="ref" value="cmdform">

</form>
    </div>

  </div>
</div>




          <div class="site" itemscope itemtype="http://schema.org/WebPage">
    
    <div class="pagehead repohead instapaper_ignore readability-menu">
      <div class="container">
        

<ul class="pagehead-actions">


  <li>
    <a href="/login?return_to=%2Fmobyfab%2FChibiOS"
    class="minibutton with-count js-toggler-target star-button tooltipped tooltipped-n"
    aria-label="You must be signed in to use this feature" rel="nofollow">
    <span class="octicon octicon-star"></span>Star
  </a>

    <a class="social-count js-social-count" href="/mobyfab/ChibiOS/stargazers">
      2
    </a>

  </li>

    <li>
      <a href="/login?return_to=%2Fmobyfab%2FChibiOS"
        class="minibutton with-count js-toggler-target fork-button tooltipped tooltipped-n"
        aria-label="You must be signed in to fork a repository" rel="nofollow">
        <span class="octicon octicon-git-branch"></span>Fork
      </a>
      <a href="/mobyfab/ChibiOS/network" class="social-count">
        87
      </a>
    </li>
</ul>

        <h1 itemscope itemtype="http://data-vocabulary.org/Breadcrumb" class="entry-title public">
          <span class="repo-label"><span>public</span></span>
          <span class="mega-octicon octicon-repo"></span>
          <span class="author">
            <a href="/mobyfab" class="url fn" itemprop="url" rel="author"><span itemprop="title">mobyfab</span></a>
          </span>
          <span class="repohead-name-divider">/</span>
          <strong><a href="/mobyfab/ChibiOS" class="js-current-repository js-repo-home-link">ChibiOS</a></strong>

          <span class="page-context-loader">
            <img alt="Octocat-spinner-32" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
          </span>

            <span class="fork-flag">
              <span class="text">forked from <a href="/mabl/ChibiOS">mabl/ChibiOS</a></span>
            </span>
        </h1>
      </div><!-- /.container -->
    </div><!-- /.repohead -->

    <div class="container">
      <div class="repository-with-sidebar repo-container new-discussion-timeline js-new-discussion-timeline  ">
        <div class="repository-sidebar clearfix">
            

<div class="sunken-menu vertical-right repo-nav js-repo-nav js-repository-container-pjax js-octicon-loaders">
  <div class="sunken-menu-contents">
    <ul class="sunken-menu-group">
      <li class="tooltipped tooltipped-w" aria-label="Code">
        <a href="/mobyfab/ChibiOS/tree/dac-3.0" aria-label="Code" class="selected js-selected-navigation-item sunken-menu-item" data-gotokey="c" data-pjax="true" data-selected-links="repo_source repo_downloads repo_commits repo_tags repo_branches /mobyfab/ChibiOS/tree/dac-3.0">
          <span class="octicon octicon-code"></span> <span class="full-word">Code</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>


      <li class="tooltipped tooltipped-w" aria-label="Pull Requests">
        <a href="/mobyfab/ChibiOS/pulls" aria-label="Pull Requests" class="js-selected-navigation-item sunken-menu-item js-disable-pjax" data-gotokey="p" data-selected-links="repo_pulls /mobyfab/ChibiOS/pulls">
            <span class="octicon octicon-git-pull-request"></span> <span class="full-word">Pull Requests</span>
            <span class='counter'>0</span>
            <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>


    </ul>
    <div class="sunken-menu-separator"></div>
    <ul class="sunken-menu-group">

      <li class="tooltipped tooltipped-w" aria-label="Pulse">
        <a href="/mobyfab/ChibiOS/pulse" aria-label="Pulse" class="js-selected-navigation-item sunken-menu-item" data-pjax="true" data-selected-links="pulse /mobyfab/ChibiOS/pulse">
          <span class="octicon octicon-pulse"></span> <span class="full-word">Pulse</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

      <li class="tooltipped tooltipped-w" aria-label="Graphs">
        <a href="/mobyfab/ChibiOS/graphs" aria-label="Graphs" class="js-selected-navigation-item sunken-menu-item" data-pjax="true" data-selected-links="repo_graphs repo_contributors /mobyfab/ChibiOS/graphs">
          <span class="octicon octicon-graph"></span> <span class="full-word">Graphs</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

      <li class="tooltipped tooltipped-w" aria-label="Network">
        <a href="/mobyfab/ChibiOS/network" aria-label="Network" class="js-selected-navigation-item sunken-menu-item js-disable-pjax" data-selected-links="repo_network /mobyfab/ChibiOS/network">
          <span class="octicon octicon-git-branch"></span> <span class="full-word">Network</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>
    </ul>


  </div>
</div>

              <div class="only-with-full-nav">
                

  

<div class="clone-url open"
  data-protocol-type="http"
  data-url="/users/set_protocol?protocol_selector=http&amp;protocol_type=clone">
  <h3><strong>HTTPS</strong> clone URL</h3>
  <div class="clone-url-box">
    <input type="text" class="clone js-url-field"
           value="https://github.com/mobyfab/ChibiOS.git" readonly="readonly">

    <span aria-label="copy to clipboard" class="js-zeroclipboard url-box-clippy minibutton zeroclipboard-button" data-clipboard-text="https://github.com/mobyfab/ChibiOS.git" data-copied-hint="copied!"><span class="octicon octicon-clippy"></span></span>
  </div>
</div>

  

<div class="clone-url "
  data-protocol-type="subversion"
  data-url="/users/set_protocol?protocol_selector=subversion&amp;protocol_type=clone">
  <h3><strong>Subversion</strong> checkout URL</h3>
  <div class="clone-url-box">
    <input type="text" class="clone js-url-field"
           value="https://github.com/mobyfab/ChibiOS" readonly="readonly">

    <span aria-label="copy to clipboard" class="js-zeroclipboard url-box-clippy minibutton zeroclipboard-button" data-clipboard-text="https://github.com/mobyfab/ChibiOS" data-copied-hint="copied!"><span class="octicon octicon-clippy"></span></span>
  </div>
</div>


<p class="clone-options">You can clone with
      <a href="#" class="js-clone-selector" data-protocol="http">HTTPS</a>
      or <a href="#" class="js-clone-selector" data-protocol="subversion">Subversion</a>.
  <span class="help tooltipped tooltipped-n" aria-label="Get help on which URL is right for you.">
    <a href="https://help.github.com/articles/which-remote-url-should-i-use">
    <span class="octicon octicon-question"></span>
    </a>
  </span>
</p>


  <a href="http://windows.github.com" class="minibutton sidebar-button">
    <span class="octicon octicon-device-desktop"></span>
    Clone in Desktop
  </a>

                <a href="/mobyfab/ChibiOS/archive/dac-3.0.zip"
                   class="minibutton sidebar-button"
                   title="Download this repository as a zip file"
                   rel="nofollow">
                  <span class="octicon octicon-cloud-download"></span>
                  Download ZIP
                </a>
              </div>
        </div><!-- /.repository-sidebar -->

        <div id="js-repo-pjax-container" class="repository-content context-loader-container" data-pjax-container>
          


<!-- blob contrib key: blob_contributors:v21:13581fbc78ff455f522b6195dc09b1f5 -->

<p title="This is a placeholder element" class="js-history-link-replace hidden"></p>

<a href="/mobyfab/ChibiOS/find/dac-3.0" data-pjax data-hotkey="t" class="js-show-file-finder" style="display:none">Show File Finder</a>

<div class="file-navigation">
  

<div class="select-menu js-menu-container js-select-menu" >
  <span class="minibutton select-menu-button js-menu-target" data-hotkey="w"
    data-master-branch="master"
    data-ref="dac-3.0"
    role="button" aria-label="Switch branches or tags" tabindex="0" aria-haspopup="true">
    <span class="octicon octicon-git-branch"></span>
    <i>branch:</i>
    <span class="js-select-button">dac-3.0</span>
  </span>

  <div class="select-menu-modal-holder js-menu-content js-navigation-container" data-pjax aria-hidden="true">

    <div class="select-menu-modal">
      <div class="select-menu-header">
        <span class="select-menu-title">Switch branches/tags</span>
        <span class="octicon octicon-remove-close js-menu-close"></span>
      </div> <!-- /.select-menu-header -->

      <div class="select-menu-filters">
        <div class="select-menu-text-filter">
          <input type="text" aria-label="Filter branches/tags" id="context-commitish-filter-field" class="js-filterable-field js-navigation-enable" placeholder="Filter branches/tags">
        </div>
        <div class="select-menu-tabs">
          <ul>
            <li class="select-menu-tab">
              <a href="#" data-tab-filter="branches" class="js-select-menu-tab">Branches</a>
            </li>
            <li class="select-menu-tab">
              <a href="#" data-tab-filter="tags" class="js-select-menu-tab">Tags</a>
            </li>
          </ul>
        </div><!-- /.select-menu-tabs -->
      </div><!-- /.select-menu-filters -->

      <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket" data-tab-filter="branches">

        <div data-filterable-for="context-commitish-filter-field" data-filterable-type="substring">


            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/chfs_test/os/hal/src/dac.c"
                 data-name="chfs_test"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="chfs_test">chfs_test</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/coldfire_dev/os/hal/src/dac.c"
                 data-name="coldfire_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="coldfire_dev">coldfire_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/condvars_test/os/hal/src/dac.c"
                 data-name="condvars_test"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="condvars_test">condvars_test</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/condvars_test_alt/os/hal/src/dac.c"
                 data-name="condvars_test_alt"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="condvars_test_alt">condvars_test_alt</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/cpp_dev/os/hal/src/dac.c"
                 data-name="cpp_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="cpp_dev">cpp_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/dac/os/hal/src/dac.c"
                 data-name="dac"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="dac">dac</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item selected">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/dac-3.0/os/hal/src/dac.c"
                 data-name="dac-3.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="dac-3.0">dac-3.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/dac-new/os/hal/src/dac.c"
                 data-name="dac-new"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="dac-new">dac-new</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/dynclk_dev/os/hal/src/dac.c"
                 data-name="dynclk_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="dynclk_dev">dynclk_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/fujitsu_dev/os/hal/src/dac.c"
                 data-name="fujitsu_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="fujitsu_dev">fujitsu_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/fujitsu_dev_2.5.0/os/hal/src/dac.c"
                 data-name="fujitsu_dev_2.5.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="fujitsu_dev_2.5.0">fujitsu_dev_2.5.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/h8s__dev/os/hal/src/dac.c"
                 data-name="h8s__dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="h8s__dev">h8s__dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/h8s_dev/os/hal/src/dac.c"
                 data-name="h8s_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="h8s_dev">h8s_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/i2c_dev/os/hal/src/dac.c"
                 data-name="i2c_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="i2c_dev">i2c_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/kernopt_dev/os/hal/src/dac.c"
                 data-name="kernopt_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="kernopt_dev">kernopt_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/lwip_test/os/hal/src/dac.c"
                 data-name="lwip_test"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="lwip_test">lwip_test</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/master/os/hal/src/dac.c"
                 data-name="master"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="master">master</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/nil_trunk/os/hal/src/dac.c"
                 data-name="nil_trunk"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="nil_trunk">nil_trunk</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/origin/master/os/hal/src/dac.c"
                 data-name="origin/master"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="origin/master">origin/master</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/portable_hal_dev/os/hal/src/dac.c"
                 data-name="portable_hal_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="portable_hal_dev">portable_hal_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/ppc_dev/os/hal/src/dac.c"
                 data-name="ppc_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="ppc_dev">ppc_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/pvd2_dev/os/hal/src/dac.c"
                 data-name="pvd2_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="pvd2_dev">pvd2_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/pvd_dev/os/hal/src/dac.c"
                 data-name="pvd_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="pvd_dev">pvd_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/rtc_dev/os/hal/src/dac.c"
                 data-name="rtc_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="rtc_dev">rtc_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/sdc_dev2/os/hal/src/dac.c"
                 data-name="sdc_dev2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="sdc_dev2">sdc_dev2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/sdio_dev/os/hal/src/dac.c"
                 data-name="sdio_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="sdio_dev">sdio_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/spc5studio_components_trunk/os/hal/src/dac.c"
                 data-name="spc5studio_components_trunk"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="spc5studio_components_trunk">spc5studio_components_trunk</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_1.0.x/os/hal/src/dac.c"
                 data-name="stable_1.0.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_1.0.x">stable_1.0.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_1.2.x/os/hal/src/dac.c"
                 data-name="stable_1.2.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_1.2.x">stable_1.2.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_1.4.x/os/hal/src/dac.c"
                 data-name="stable_1.4.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_1.4.x">stable_1.4.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_2.0.x/os/hal/src/dac.c"
                 data-name="stable_2.0.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_2.0.x">stable_2.0.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_2.2.x/os/hal/src/dac.c"
                 data-name="stable_2.2.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_2.2.x">stable_2.2.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_2.4.x/os/hal/src/dac.c"
                 data-name="stable_2.4.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_2.4.x">stable_2.4.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_2.6.x/os/hal/src/dac.c"
                 data-name="stable_2.6.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_2.6.x">stable_2.6.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stm8_cosmic_dev/os/hal/src/dac.c"
                 data-name="stm8_cosmic_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stm8_cosmic_dev">stm8_cosmic_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stm8_dev/os/hal/src/dac.c"
                 data-name="stm8_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stm8_dev">stm8_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stm8_hal/os/hal/src/dac.c"
                 data-name="stm8_hal"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stm8_hal">stm8_hal</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stm32_ethernet_test/os/hal/src/dac.c"
                 data-name="stm32_ethernet_test"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stm32_ethernet_test">stm32_ethernet_test</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stm32_ethernet_wrapper/os/hal/src/dac.c"
                 data-name="stm32_ethernet_wrapper"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stm32_ethernet_wrapper">stm32_ethernet_wrapper</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stm32f2xx_dev/os/hal/src/dac.c"
                 data-name="stm32f2xx_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stm32f2xx_dev">stm32f2xx_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/nil_0.0.1/os/hal/src/dac.c"
                 data-name="tags/nil_0.0.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/nil_0.0.1">tags/nil_0.0.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/spc5studio_components_1.1.0.RC1/os/hal/src/dac.c"
                 data-name="tags/spc5studio_components_1.1.0.RC1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/spc5studio_components_1.1.0.RC1">tags/spc5studio_components_1.1.0.RC1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/spc5studio_components_frozen_1/os/hal/src/dac.c"
                 data-name="tags/spc5studio_components_frozen_1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/spc5studio_components_frozen_1">tags/spc5studio_components_frozen_1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.1.0/os/hal/src/dac.c"
                 data-name="tags/ver_0.1.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.1.0">tags/ver_0.1.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.1.1/os/hal/src/dac.c"
                 data-name="tags/ver_0.1.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.1.1">tags/ver_0.1.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.2.0/os/hal/src/dac.c"
                 data-name="tags/ver_0.2.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.2.0">tags/ver_0.2.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.2.1/os/hal/src/dac.c"
                 data-name="tags/ver_0.2.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.2.1">tags/ver_0.2.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.0/os/hal/src/dac.c"
                 data-name="tags/ver_0.3.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.0">tags/ver_0.3.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.1/os/hal/src/dac.c"
                 data-name="tags/ver_0.3.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.1">tags/ver_0.3.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.2/os/hal/src/dac.c"
                 data-name="tags/ver_0.3.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.2">tags/ver_0.3.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.3/os/hal/src/dac.c"
                 data-name="tags/ver_0.3.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.3">tags/ver_0.3.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.4/os/hal/src/dac.c"
                 data-name="tags/ver_0.3.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.4">tags/ver_0.3.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.5/os/hal/src/dac.c"
                 data-name="tags/ver_0.3.5"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.5">tags/ver_0.3.5</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.6/os/hal/src/dac.c"
                 data-name="tags/ver_0.3.6"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.6">tags/ver_0.3.6</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.4.0/os/hal/src/dac.c"
                 data-name="tags/ver_0.4.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.4.0">tags/ver_0.4.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.4.1/os/hal/src/dac.c"
                 data-name="tags/ver_0.4.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.4.1">tags/ver_0.4.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.4.2/os/hal/src/dac.c"
                 data-name="tags/ver_0.4.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.4.2">tags/ver_0.4.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.4.4/os/hal/src/dac.c"
                 data-name="tags/ver_0.4.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.4.4">tags/ver_0.4.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.4.5/os/hal/src/dac.c"
                 data-name="tags/ver_0.4.5"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.4.5">tags/ver_0.4.5</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.5.0/os/hal/src/dac.c"
                 data-name="tags/ver_0.5.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.5.0">tags/ver_0.5.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.5.1/os/hal/src/dac.c"
                 data-name="tags/ver_0.5.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.5.1">tags/ver_0.5.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.5.2/os/hal/src/dac.c"
                 data-name="tags/ver_0.5.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.5.2">tags/ver_0.5.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.5.3/os/hal/src/dac.c"
                 data-name="tags/ver_0.5.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.5.3">tags/ver_0.5.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.5.4/os/hal/src/dac.c"
                 data-name="tags/ver_0.5.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.5.4">tags/ver_0.5.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.0/os/hal/src/dac.c"
                 data-name="tags/ver_0.6.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.0">tags/ver_0.6.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.1/os/hal/src/dac.c"
                 data-name="tags/ver_0.6.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.1">tags/ver_0.6.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.2/os/hal/src/dac.c"
                 data-name="tags/ver_0.6.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.2">tags/ver_0.6.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.4/os/hal/src/dac.c"
                 data-name="tags/ver_0.6.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.4">tags/ver_0.6.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.5/os/hal/src/dac.c"
                 data-name="tags/ver_0.6.5"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.5">tags/ver_0.6.5</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.6/os/hal/src/dac.c"
                 data-name="tags/ver_0.6.6"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.6">tags/ver_0.6.6</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.7/os/hal/src/dac.c"
                 data-name="tags/ver_0.6.7"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.7">tags/ver_0.6.7</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.8/os/hal/src/dac.c"
                 data-name="tags/ver_0.6.8"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.8">tags/ver_0.6.8</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.9/os/hal/src/dac.c"
                 data-name="tags/ver_0.6.9"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.9">tags/ver_0.6.9</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.10/os/hal/src/dac.c"
                 data-name="tags/ver_0.6.10"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.10">tags/ver_0.6.10</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.7.0/os/hal/src/dac.c"
                 data-name="tags/ver_0.7.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.7.0">tags/ver_0.7.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.7.1/os/hal/src/dac.c"
                 data-name="tags/ver_0.7.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.7.1">tags/ver_0.7.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.7.2/os/hal/src/dac.c"
                 data-name="tags/ver_0.7.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.7.2">tags/ver_0.7.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.7.3/os/hal/src/dac.c"
                 data-name="tags/ver_0.7.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.7.3">tags/ver_0.7.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.8.0/os/hal/src/dac.c"
                 data-name="tags/ver_0.8.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.8.0">tags/ver_0.8.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.8.1/os/hal/src/dac.c"
                 data-name="tags/ver_0.8.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.8.1">tags/ver_0.8.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.8.2/os/hal/src/dac.c"
                 data-name="tags/ver_0.8.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.8.2">tags/ver_0.8.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.8.3/os/hal/src/dac.c"
                 data-name="tags/ver_0.8.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.8.3">tags/ver_0.8.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.0.0/os/hal/src/dac.c"
                 data-name="tags/ver_1.0.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.0.0">tags/ver_1.0.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.0.0rc1/os/hal/src/dac.c"
                 data-name="tags/ver_1.0.0rc1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.0.0rc1">tags/ver_1.0.0rc1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.0.0rc2/os/hal/src/dac.c"
                 data-name="tags/ver_1.0.0rc2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.0.0rc2">tags/ver_1.0.0rc2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.0.0rc3/os/hal/src/dac.c"
                 data-name="tags/ver_1.0.0rc3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.0.0rc3">tags/ver_1.0.0rc3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.0.1/os/hal/src/dac.c"
                 data-name="tags/ver_1.0.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.0.1">tags/ver_1.0.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.0.2/os/hal/src/dac.c"
                 data-name="tags/ver_1.0.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.0.2">tags/ver_1.0.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.1.0unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.1.0unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.1.0unstable">tags/ver_1.1.0unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.1.1unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.1.1unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.1.1unstable">tags/ver_1.1.1unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.1.2unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.1.2unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.1.2unstable">tags/ver_1.1.2unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.1.3unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.1.3unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.1.3unstable">tags/ver_1.1.3unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.2.0/os/hal/src/dac.c"
                 data-name="tags/ver_1.2.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.2.0">tags/ver_1.2.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.2.1/os/hal/src/dac.c"
                 data-name="tags/ver_1.2.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.2.1">tags/ver_1.2.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.2.2/os/hal/src/dac.c"
                 data-name="tags/ver_1.2.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.2.2">tags/ver_1.2.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.2.3/os/hal/src/dac.c"
                 data-name="tags/ver_1.2.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.2.3">tags/ver_1.2.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.2.4/os/hal/src/dac.c"
                 data-name="tags/ver_1.2.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.2.4">tags/ver_1.2.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.0unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.3.0unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.0unstable">tags/ver_1.3.0unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.1unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.3.1unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.1unstable">tags/ver_1.3.1unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.2temporary/os/hal/src/dac.c"
                 data-name="tags/ver_1.3.2temporary"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.2temporary">tags/ver_1.3.2temporary</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.2unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.3.2unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.2unstable">tags/ver_1.3.2unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.3unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.3.3unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.3unstable">tags/ver_1.3.3unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.4unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.3.4unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.4unstable">tags/ver_1.3.4unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.5unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.3.5unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.5unstable">tags/ver_1.3.5unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.6unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.3.6unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.6unstable">tags/ver_1.3.6unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.7unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.3.7unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.7unstable">tags/ver_1.3.7unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.8unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.3.8unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.8unstable">tags/ver_1.3.8unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.4.0/os/hal/src/dac.c"
                 data-name="tags/ver_1.4.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.4.0">tags/ver_1.4.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.4.1/os/hal/src/dac.c"
                 data-name="tags/ver_1.4.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.4.1">tags/ver_1.4.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.4.2/os/hal/src/dac.c"
                 data-name="tags/ver_1.4.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.4.2">tags/ver_1.4.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.4.3/os/hal/src/dac.c"
                 data-name="tags/ver_1.4.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.4.3">tags/ver_1.4.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.0unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.5.0unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.0unstable">tags/ver_1.5.0unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.1unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.5.1unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.1unstable">tags/ver_1.5.1unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.2unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.5.2unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.2unstable">tags/ver_1.5.2unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.3unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.5.3unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.3unstable">tags/ver_1.5.3unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.4unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.5.4unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.4unstable">tags/ver_1.5.4unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.5unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.5.5unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.5unstable">tags/ver_1.5.5unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.6unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.5.6unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.6unstable">tags/ver_1.5.6unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.7unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.5.7unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.7unstable">tags/ver_1.5.7unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.8unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.5.8unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.8unstable">tags/ver_1.5.8unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.9unstable/os/hal/src/dac.c"
                 data-name="tags/ver_1.5.9unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.9unstable">tags/ver_1.5.9unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.0/os/hal/src/dac.c"
                 data-name="tags/ver_2.0.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.0">tags/ver_2.0.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.1/os/hal/src/dac.c"
                 data-name="tags/ver_2.0.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.1">tags/ver_2.0.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.2/os/hal/src/dac.c"
                 data-name="tags/ver_2.0.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.2">tags/ver_2.0.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.3/os/hal/src/dac.c"
                 data-name="tags/ver_2.0.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.3">tags/ver_2.0.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.4/os/hal/src/dac.c"
                 data-name="tags/ver_2.0.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.4">tags/ver_2.0.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.5/os/hal/src/dac.c"
                 data-name="tags/ver_2.0.5"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.5">tags/ver_2.0.5</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.6/os/hal/src/dac.c"
                 data-name="tags/ver_2.0.6"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.6">tags/ver_2.0.6</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.7/os/hal/src/dac.c"
                 data-name="tags/ver_2.0.7"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.7">tags/ver_2.0.7</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.8/os/hal/src/dac.c"
                 data-name="tags/ver_2.0.8"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.8">tags/ver_2.0.8</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.9/os/hal/src/dac.c"
                 data-name="tags/ver_2.0.9"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.9">tags/ver_2.0.9</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.10/os/hal/src/dac.c"
                 data-name="tags/ver_2.0.10"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.10">tags/ver_2.0.10</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.0unstable/os/hal/src/dac.c"
                 data-name="tags/ver_2.1.0unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.0unstable">tags/ver_2.1.0unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.1unstable/os/hal/src/dac.c"
                 data-name="tags/ver_2.1.1unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.1unstable">tags/ver_2.1.1unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.2unstable/os/hal/src/dac.c"
                 data-name="tags/ver_2.1.2unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.2unstable">tags/ver_2.1.2unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.3unstable/os/hal/src/dac.c"
                 data-name="tags/ver_2.1.3unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.3unstable">tags/ver_2.1.3unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.4unstable/os/hal/src/dac.c"
                 data-name="tags/ver_2.1.4unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.4unstable">tags/ver_2.1.4unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.5unstable/os/hal/src/dac.c"
                 data-name="tags/ver_2.1.5unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.5unstable">tags/ver_2.1.5unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.6unstable/os/hal/src/dac.c"
                 data-name="tags/ver_2.1.6unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.6unstable">tags/ver_2.1.6unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.7unstable/os/hal/src/dac.c"
                 data-name="tags/ver_2.1.7unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.7unstable">tags/ver_2.1.7unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.0/os/hal/src/dac.c"
                 data-name="tags/ver_2.2.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.0">tags/ver_2.2.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.1/os/hal/src/dac.c"
                 data-name="tags/ver_2.2.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.1">tags/ver_2.2.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.2/os/hal/src/dac.c"
                 data-name="tags/ver_2.2.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.2">tags/ver_2.2.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.3/os/hal/src/dac.c"
                 data-name="tags/ver_2.2.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.3">tags/ver_2.2.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.4/os/hal/src/dac.c"
                 data-name="tags/ver_2.2.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.4">tags/ver_2.2.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.5/os/hal/src/dac.c"
                 data-name="tags/ver_2.2.5"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.5">tags/ver_2.2.5</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.6/os/hal/src/dac.c"
                 data-name="tags/ver_2.2.6"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.6">tags/ver_2.2.6</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.7/os/hal/src/dac.c"
                 data-name="tags/ver_2.2.7"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.7">tags/ver_2.2.7</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.8/os/hal/src/dac.c"
                 data-name="tags/ver_2.2.8"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.8">tags/ver_2.2.8</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.9/os/hal/src/dac.c"
                 data-name="tags/ver_2.2.9"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.9">tags/ver_2.2.9</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.3.0/os/hal/src/dac.c"
                 data-name="tags/ver_2.3.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.3.0">tags/ver_2.3.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.3.1/os/hal/src/dac.c"
                 data-name="tags/ver_2.3.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.3.1">tags/ver_2.3.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.3.2/os/hal/src/dac.c"
                 data-name="tags/ver_2.3.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.3.2">tags/ver_2.3.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.3.3/os/hal/src/dac.c"
                 data-name="tags/ver_2.3.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.3.3">tags/ver_2.3.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.3.4/os/hal/src/dac.c"
                 data-name="tags/ver_2.3.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.3.4">tags/ver_2.3.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.4.0/os/hal/src/dac.c"
                 data-name="tags/ver_2.4.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.4.0">tags/ver_2.4.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.4.1/os/hal/src/dac.c"
                 data-name="tags/ver_2.4.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.4.1">tags/ver_2.4.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.4.2/os/hal/src/dac.c"
                 data-name="tags/ver_2.4.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.4.2">tags/ver_2.4.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.4.3/os/hal/src/dac.c"
                 data-name="tags/ver_2.4.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.4.3">tags/ver_2.4.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.5.0/os/hal/src/dac.c"
                 data-name="tags/ver_2.5.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.5.0">tags/ver_2.5.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.5.1/os/hal/src/dac.c"
                 data-name="tags/ver_2.5.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.5.1">tags/ver_2.5.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/trunk/os/hal/src/dac.c"
                 data-name="trunk"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="trunk">trunk</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tsc/os/hal/src/dac.c"
                 data-name="tsc"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tsc">tsc</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/unstable_1.1/os/hal/src/dac.c"
                 data-name="unstable_1.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="unstable_1.1">unstable_1.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/usb/os/hal/src/dac.c"
                 data-name="usb"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="usb">usb</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/usb_dev/os/hal/src/dac.c"
                 data-name="usb_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="usb_dev">usb_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/utils_dev/os/hal/src/dac.c"
                 data-name="utils_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="utils_dev">utils_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/various/os/hal/src/dac.c"
                 data-name="various"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="various">various</a>
            </div> <!-- /.select-menu-item -->
        </div>

          <div class="select-menu-no-results">Nothing to show</div>
      </div> <!-- /.select-menu-list -->

      <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket" data-tab-filter="tags">
        <div data-filterable-for="context-commitish-filter-field" data-filterable-type="substring">


        </div>

        <div class="select-menu-no-results">Nothing to show</div>
      </div> <!-- /.select-menu-list -->

    </div> <!-- /.select-menu-modal -->
  </div> <!-- /.select-menu-modal-holder -->
</div> <!-- /.select-menu -->

  <div class="breadcrumb">
    <span class='repo-root js-repo-root'><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/mobyfab/ChibiOS/tree/dac-3.0" data-branch="dac-3.0" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">ChibiOS</span></a></span></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/mobyfab/ChibiOS/tree/dac-3.0/os" data-branch="dac-3.0" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">os</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/mobyfab/ChibiOS/tree/dac-3.0/os/hal" data-branch="dac-3.0" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">hal</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/mobyfab/ChibiOS/tree/dac-3.0/os/hal/src" data-branch="dac-3.0" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">src</span></a></span><span class="separator"> / </span><strong class="final-path">dac.c</strong> <span aria-label="copy to clipboard" class="js-zeroclipboard minibutton zeroclipboard-button" data-clipboard-text="os/hal/src/dac.c" data-copied-hint="copied!"><span class="octicon octicon-clippy"></span></span>
  </div>
</div>


  <div class="commit commit-loader file-history-tease js-deferred-content" data-url="/mobyfab/ChibiOS/contributors/dac-3.0/os/hal/src/dac.c">
    Fetching contributors…

    <div class="participation">
      <p class="loader-loading"><img alt="Octocat-spinner-32-eaf2f5" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32-EAF2F5.gif" width="16" /></p>
      <p class="loader-error">Cannot retrieve contributors at this time</p>
    </div>
  </div>

<div class="file-box">
  <div class="file">
    <div class="meta clearfix">
      <div class="info file-name">
        <span class="icon"><b class="octicon octicon-file-text"></b></span>
        <span class="mode" title="File Mode">file</span>
        <span class="meta-divider"></span>
          <span>325 lines (281 sloc)</span>
          <span class="meta-divider"></span>
        <span>10.304 kb</span>
      </div>
      <div class="actions">
        <div class="button-group">
            <a class="minibutton tooltipped tooltipped-w"
               href="http://windows.github.com" aria-label="Open this file in GitHub for Windows">
                <span class="octicon octicon-device-desktop"></span> Open
            </a>
              <a class="minibutton disabled tooltipped tooltipped-w" href="#"
                 aria-label="You must be signed in to make or propose changes">Edit</a>
          <a href="/mobyfab/ChibiOS/raw/dac-3.0/os/hal/src/dac.c" class="button minibutton " id="raw-url">Raw</a>
            <a href="/mobyfab/ChibiOS/blame/dac-3.0/os/hal/src/dac.c" class="button minibutton js-update-url-with-hash">Blame</a>
          <a href="/mobyfab/ChibiOS/commits/dac-3.0/os/hal/src/dac.c" class="button minibutton " rel="nofollow">History</a>
        </div><!-- /.button-group -->
          <a class="minibutton danger disabled empty-icon tooltipped tooltipped-w" href="#"
             aria-label="You must be signed in to make or propose changes">
          Delete
        </a>
      </div><!-- /.actions -->
    </div>
        <div class="blob-wrapper data type-c js-blob-data">
        <table class="file-code file-diff tab-size-8">
          <tr class="file-code-line">
            <td class="blob-line-nums">
              <span id="L1" rel="#L1">1</span>
<span id="L2" rel="#L2">2</span>
<span id="L3" rel="#L3">3</span>
<span id="L4" rel="#L4">4</span>
<span id="L5" rel="#L5">5</span>
<span id="L6" rel="#L6">6</span>
<span id="L7" rel="#L7">7</span>
<span id="L8" rel="#L8">8</span>
<span id="L9" rel="#L9">9</span>
<span id="L10" rel="#L10">10</span>
<span id="L11" rel="#L11">11</span>
<span id="L12" rel="#L12">12</span>
<span id="L13" rel="#L13">13</span>
<span id="L14" rel="#L14">14</span>
<span id="L15" rel="#L15">15</span>
<span id="L16" rel="#L16">16</span>
<span id="L17" rel="#L17">17</span>
<span id="L18" rel="#L18">18</span>
<span id="L19" rel="#L19">19</span>
<span id="L20" rel="#L20">20</span>
<span id="L21" rel="#L21">21</span>
<span id="L22" rel="#L22">22</span>
<span id="L23" rel="#L23">23</span>
<span id="L24" rel="#L24">24</span>
<span id="L25" rel="#L25">25</span>
<span id="L26" rel="#L26">26</span>
<span id="L27" rel="#L27">27</span>
<span id="L28" rel="#L28">28</span>
<span id="L29" rel="#L29">29</span>
<span id="L30" rel="#L30">30</span>
<span id="L31" rel="#L31">31</span>
<span id="L32" rel="#L32">32</span>
<span id="L33" rel="#L33">33</span>
<span id="L34" rel="#L34">34</span>
<span id="L35" rel="#L35">35</span>
<span id="L36" rel="#L36">36</span>
<span id="L37" rel="#L37">37</span>
<span id="L38" rel="#L38">38</span>
<span id="L39" rel="#L39">39</span>
<span id="L40" rel="#L40">40</span>
<span id="L41" rel="#L41">41</span>
<span id="L42" rel="#L42">42</span>
<span id="L43" rel="#L43">43</span>
<span id="L44" rel="#L44">44</span>
<span id="L45" rel="#L45">45</span>
<span id="L46" rel="#L46">46</span>
<span id="L47" rel="#L47">47</span>
<span id="L48" rel="#L48">48</span>
<span id="L49" rel="#L49">49</span>
<span id="L50" rel="#L50">50</span>
<span id="L51" rel="#L51">51</span>
<span id="L52" rel="#L52">52</span>
<span id="L53" rel="#L53">53</span>
<span id="L54" rel="#L54">54</span>
<span id="L55" rel="#L55">55</span>
<span id="L56" rel="#L56">56</span>
<span id="L57" rel="#L57">57</span>
<span id="L58" rel="#L58">58</span>
<span id="L59" rel="#L59">59</span>
<span id="L60" rel="#L60">60</span>
<span id="L61" rel="#L61">61</span>
<span id="L62" rel="#L62">62</span>
<span id="L63" rel="#L63">63</span>
<span id="L64" rel="#L64">64</span>
<span id="L65" rel="#L65">65</span>
<span id="L66" rel="#L66">66</span>
<span id="L67" rel="#L67">67</span>
<span id="L68" rel="#L68">68</span>
<span id="L69" rel="#L69">69</span>
<span id="L70" rel="#L70">70</span>
<span id="L71" rel="#L71">71</span>
<span id="L72" rel="#L72">72</span>
<span id="L73" rel="#L73">73</span>
<span id="L74" rel="#L74">74</span>
<span id="L75" rel="#L75">75</span>
<span id="L76" rel="#L76">76</span>
<span id="L77" rel="#L77">77</span>
<span id="L78" rel="#L78">78</span>
<span id="L79" rel="#L79">79</span>
<span id="L80" rel="#L80">80</span>
<span id="L81" rel="#L81">81</span>
<span id="L82" rel="#L82">82</span>
<span id="L83" rel="#L83">83</span>
<span id="L84" rel="#L84">84</span>
<span id="L85" rel="#L85">85</span>
<span id="L86" rel="#L86">86</span>
<span id="L87" rel="#L87">87</span>
<span id="L88" rel="#L88">88</span>
<span id="L89" rel="#L89">89</span>
<span id="L90" rel="#L90">90</span>
<span id="L91" rel="#L91">91</span>
<span id="L92" rel="#L92">92</span>
<span id="L93" rel="#L93">93</span>
<span id="L94" rel="#L94">94</span>
<span id="L95" rel="#L95">95</span>
<span id="L96" rel="#L96">96</span>
<span id="L97" rel="#L97">97</span>
<span id="L98" rel="#L98">98</span>
<span id="L99" rel="#L99">99</span>
<span id="L100" rel="#L100">100</span>
<span id="L101" rel="#L101">101</span>
<span id="L102" rel="#L102">102</span>
<span id="L103" rel="#L103">103</span>
<span id="L104" rel="#L104">104</span>
<span id="L105" rel="#L105">105</span>
<span id="L106" rel="#L106">106</span>
<span id="L107" rel="#L107">107</span>
<span id="L108" rel="#L108">108</span>
<span id="L109" rel="#L109">109</span>
<span id="L110" rel="#L110">110</span>
<span id="L111" rel="#L111">111</span>
<span id="L112" rel="#L112">112</span>
<span id="L113" rel="#L113">113</span>
<span id="L114" rel="#L114">114</span>
<span id="L115" rel="#L115">115</span>
<span id="L116" rel="#L116">116</span>
<span id="L117" rel="#L117">117</span>
<span id="L118" rel="#L118">118</span>
<span id="L119" rel="#L119">119</span>
<span id="L120" rel="#L120">120</span>
<span id="L121" rel="#L121">121</span>
<span id="L122" rel="#L122">122</span>
<span id="L123" rel="#L123">123</span>
<span id="L124" rel="#L124">124</span>
<span id="L125" rel="#L125">125</span>
<span id="L126" rel="#L126">126</span>
<span id="L127" rel="#L127">127</span>
<span id="L128" rel="#L128">128</span>
<span id="L129" rel="#L129">129</span>
<span id="L130" rel="#L130">130</span>
<span id="L131" rel="#L131">131</span>
<span id="L132" rel="#L132">132</span>
<span id="L133" rel="#L133">133</span>
<span id="L134" rel="#L134">134</span>
<span id="L135" rel="#L135">135</span>
<span id="L136" rel="#L136">136</span>
<span id="L137" rel="#L137">137</span>
<span id="L138" rel="#L138">138</span>
<span id="L139" rel="#L139">139</span>
<span id="L140" rel="#L140">140</span>
<span id="L141" rel="#L141">141</span>
<span id="L142" rel="#L142">142</span>
<span id="L143" rel="#L143">143</span>
<span id="L144" rel="#L144">144</span>
<span id="L145" rel="#L145">145</span>
<span id="L146" rel="#L146">146</span>
<span id="L147" rel="#L147">147</span>
<span id="L148" rel="#L148">148</span>
<span id="L149" rel="#L149">149</span>
<span id="L150" rel="#L150">150</span>
<span id="L151" rel="#L151">151</span>
<span id="L152" rel="#L152">152</span>
<span id="L153" rel="#L153">153</span>
<span id="L154" rel="#L154">154</span>
<span id="L155" rel="#L155">155</span>
<span id="L156" rel="#L156">156</span>
<span id="L157" rel="#L157">157</span>
<span id="L158" rel="#L158">158</span>
<span id="L159" rel="#L159">159</span>
<span id="L160" rel="#L160">160</span>
<span id="L161" rel="#L161">161</span>
<span id="L162" rel="#L162">162</span>
<span id="L163" rel="#L163">163</span>
<span id="L164" rel="#L164">164</span>
<span id="L165" rel="#L165">165</span>
<span id="L166" rel="#L166">166</span>
<span id="L167" rel="#L167">167</span>
<span id="L168" rel="#L168">168</span>
<span id="L169" rel="#L169">169</span>
<span id="L170" rel="#L170">170</span>
<span id="L171" rel="#L171">171</span>
<span id="L172" rel="#L172">172</span>
<span id="L173" rel="#L173">173</span>
<span id="L174" rel="#L174">174</span>
<span id="L175" rel="#L175">175</span>
<span id="L176" rel="#L176">176</span>
<span id="L177" rel="#L177">177</span>
<span id="L178" rel="#L178">178</span>
<span id="L179" rel="#L179">179</span>
<span id="L180" rel="#L180">180</span>
<span id="L181" rel="#L181">181</span>
<span id="L182" rel="#L182">182</span>
<span id="L183" rel="#L183">183</span>
<span id="L184" rel="#L184">184</span>
<span id="L185" rel="#L185">185</span>
<span id="L186" rel="#L186">186</span>
<span id="L187" rel="#L187">187</span>
<span id="L188" rel="#L188">188</span>
<span id="L189" rel="#L189">189</span>
<span id="L190" rel="#L190">190</span>
<span id="L191" rel="#L191">191</span>
<span id="L192" rel="#L192">192</span>
<span id="L193" rel="#L193">193</span>
<span id="L194" rel="#L194">194</span>
<span id="L195" rel="#L195">195</span>
<span id="L196" rel="#L196">196</span>
<span id="L197" rel="#L197">197</span>
<span id="L198" rel="#L198">198</span>
<span id="L199" rel="#L199">199</span>
<span id="L200" rel="#L200">200</span>
<span id="L201" rel="#L201">201</span>
<span id="L202" rel="#L202">202</span>
<span id="L203" rel="#L203">203</span>
<span id="L204" rel="#L204">204</span>
<span id="L205" rel="#L205">205</span>
<span id="L206" rel="#L206">206</span>
<span id="L207" rel="#L207">207</span>
<span id="L208" rel="#L208">208</span>
<span id="L209" rel="#L209">209</span>
<span id="L210" rel="#L210">210</span>
<span id="L211" rel="#L211">211</span>
<span id="L212" rel="#L212">212</span>
<span id="L213" rel="#L213">213</span>
<span id="L214" rel="#L214">214</span>
<span id="L215" rel="#L215">215</span>
<span id="L216" rel="#L216">216</span>
<span id="L217" rel="#L217">217</span>
<span id="L218" rel="#L218">218</span>
<span id="L219" rel="#L219">219</span>
<span id="L220" rel="#L220">220</span>
<span id="L221" rel="#L221">221</span>
<span id="L222" rel="#L222">222</span>
<span id="L223" rel="#L223">223</span>
<span id="L224" rel="#L224">224</span>
<span id="L225" rel="#L225">225</span>
<span id="L226" rel="#L226">226</span>
<span id="L227" rel="#L227">227</span>
<span id="L228" rel="#L228">228</span>
<span id="L229" rel="#L229">229</span>
<span id="L230" rel="#L230">230</span>
<span id="L231" rel="#L231">231</span>
<span id="L232" rel="#L232">232</span>
<span id="L233" rel="#L233">233</span>
<span id="L234" rel="#L234">234</span>
<span id="L235" rel="#L235">235</span>
<span id="L236" rel="#L236">236</span>
<span id="L237" rel="#L237">237</span>
<span id="L238" rel="#L238">238</span>
<span id="L239" rel="#L239">239</span>
<span id="L240" rel="#L240">240</span>
<span id="L241" rel="#L241">241</span>
<span id="L242" rel="#L242">242</span>
<span id="L243" rel="#L243">243</span>
<span id="L244" rel="#L244">244</span>
<span id="L245" rel="#L245">245</span>
<span id="L246" rel="#L246">246</span>
<span id="L247" rel="#L247">247</span>
<span id="L248" rel="#L248">248</span>
<span id="L249" rel="#L249">249</span>
<span id="L250" rel="#L250">250</span>
<span id="L251" rel="#L251">251</span>
<span id="L252" rel="#L252">252</span>
<span id="L253" rel="#L253">253</span>
<span id="L254" rel="#L254">254</span>
<span id="L255" rel="#L255">255</span>
<span id="L256" rel="#L256">256</span>
<span id="L257" rel="#L257">257</span>
<span id="L258" rel="#L258">258</span>
<span id="L259" rel="#L259">259</span>
<span id="L260" rel="#L260">260</span>
<span id="L261" rel="#L261">261</span>
<span id="L262" rel="#L262">262</span>
<span id="L263" rel="#L263">263</span>
<span id="L264" rel="#L264">264</span>
<span id="L265" rel="#L265">265</span>
<span id="L266" rel="#L266">266</span>
<span id="L267" rel="#L267">267</span>
<span id="L268" rel="#L268">268</span>
<span id="L269" rel="#L269">269</span>
<span id="L270" rel="#L270">270</span>
<span id="L271" rel="#L271">271</span>
<span id="L272" rel="#L272">272</span>
<span id="L273" rel="#L273">273</span>
<span id="L274" rel="#L274">274</span>
<span id="L275" rel="#L275">275</span>
<span id="L276" rel="#L276">276</span>
<span id="L277" rel="#L277">277</span>
<span id="L278" rel="#L278">278</span>
<span id="L279" rel="#L279">279</span>
<span id="L280" rel="#L280">280</span>
<span id="L281" rel="#L281">281</span>
<span id="L282" rel="#L282">282</span>
<span id="L283" rel="#L283">283</span>
<span id="L284" rel="#L284">284</span>
<span id="L285" rel="#L285">285</span>
<span id="L286" rel="#L286">286</span>
<span id="L287" rel="#L287">287</span>
<span id="L288" rel="#L288">288</span>
<span id="L289" rel="#L289">289</span>
<span id="L290" rel="#L290">290</span>
<span id="L291" rel="#L291">291</span>
<span id="L292" rel="#L292">292</span>
<span id="L293" rel="#L293">293</span>
<span id="L294" rel="#L294">294</span>
<span id="L295" rel="#L295">295</span>
<span id="L296" rel="#L296">296</span>
<span id="L297" rel="#L297">297</span>
<span id="L298" rel="#L298">298</span>
<span id="L299" rel="#L299">299</span>
<span id="L300" rel="#L300">300</span>
<span id="L301" rel="#L301">301</span>
<span id="L302" rel="#L302">302</span>
<span id="L303" rel="#L303">303</span>
<span id="L304" rel="#L304">304</span>
<span id="L305" rel="#L305">305</span>
<span id="L306" rel="#L306">306</span>
<span id="L307" rel="#L307">307</span>
<span id="L308" rel="#L308">308</span>
<span id="L309" rel="#L309">309</span>
<span id="L310" rel="#L310">310</span>
<span id="L311" rel="#L311">311</span>
<span id="L312" rel="#L312">312</span>
<span id="L313" rel="#L313">313</span>
<span id="L314" rel="#L314">314</span>
<span id="L315" rel="#L315">315</span>
<span id="L316" rel="#L316">316</span>
<span id="L317" rel="#L317">317</span>
<span id="L318" rel="#L318">318</span>
<span id="L319" rel="#L319">319</span>
<span id="L320" rel="#L320">320</span>
<span id="L321" rel="#L321">321</span>
<span id="L322" rel="#L322">322</span>
<span id="L323" rel="#L323">323</span>
<span id="L324" rel="#L324">324</span>

            </td>
            <td class="blob-line-code"><div class="code-body highlight"><pre><div class='line' id='LC1'><span class="cm">/*</span></div><div class='line' id='LC2'><span class="cm">    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,</span></div><div class='line' id='LC3'><span class="cm">                 2011,2012 Giovanni Di Sirio.</span></div><div class='line' id='LC4'><br/></div><div class='line' id='LC5'><span class="cm">    This file is part of ChibiOS/RT.</span></div><div class='line' id='LC6'><br/></div><div class='line' id='LC7'><span class="cm">    ChibiOS/RT is free software; you can redistribute it and/or modify</span></div><div class='line' id='LC8'><span class="cm">    it under the terms of the GNU General Public License as published by</span></div><div class='line' id='LC9'><span class="cm">    the Free Software Foundation; either version 3 of the License, or</span></div><div class='line' id='LC10'><span class="cm">    (at your option) any later version.</span></div><div class='line' id='LC11'><br/></div><div class='line' id='LC12'><span class="cm">    ChibiOS/RT is distributed in the hope that it will be useful,</span></div><div class='line' id='LC13'><span class="cm">    but WITHOUT ANY WARRANTY; without even the implied warranty of</span></div><div class='line' id='LC14'><span class="cm">    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the</span></div><div class='line' id='LC15'><span class="cm">    GNU General Public License for more details.</span></div><div class='line' id='LC16'><br/></div><div class='line' id='LC17'><span class="cm">    You should have received a copy of the GNU General Public License</span></div><div class='line' id='LC18'><span class="cm">    along with this program.  If not, see &lt;http://www.gnu.org/licenses/&gt;.</span></div><div class='line' id='LC19'><span class="cm">*/</span></div><div class='line' id='LC20'><br/></div><div class='line' id='LC21'><span class="cm">/**</span></div><div class='line' id='LC22'><span class="cm"> * @file    dac.c</span></div><div class='line' id='LC23'><span class="cm"> * @brief   DAC Driver code.</span></div><div class='line' id='LC24'><span class="cm"> *</span></div><div class='line' id='LC25'><span class="cm"> * @addtogroup DAC</span></div><div class='line' id='LC26'><span class="cm"> * @{</span></div><div class='line' id='LC27'><span class="cm"> */</span></div><div class='line' id='LC28'><br/></div><div class='line' id='LC29'><span class="cp">#include &quot;hal.h&quot;</span></div><div class='line' id='LC30'><br/></div><div class='line' id='LC31'><span class="cp">#if HAL_USE_DAC || defined(__DOXYGEN__)</span></div><div class='line' id='LC32'><br/></div><div class='line' id='LC33'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC34'><span class="cm">/* Driver local definitions.                                                 */</span></div><div class='line' id='LC35'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC36'><br/></div><div class='line' id='LC37'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC38'><span class="cm">/* Driver exported variables.                                                */</span></div><div class='line' id='LC39'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC40'><br/></div><div class='line' id='LC41'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC42'><span class="cm">/* Driver local variables.                                                   */</span></div><div class='line' id='LC43'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC44'><br/></div><div class='line' id='LC45'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC46'><span class="cm">/* Driver local functions.                                                   */</span></div><div class='line' id='LC47'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC48'><br/></div><div class='line' id='LC49'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC50'><span class="cm">/* Driver exported functions.                                                */</span></div><div class='line' id='LC51'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC52'><br/></div><div class='line' id='LC53'><span class="cm">/**</span></div><div class='line' id='LC54'><span class="cm"> * @brief   DAC Driver initialization.</span></div><div class='line' id='LC55'><span class="cm"> * @note    This function is implicitly invoked by @p halInit(), there is</span></div><div class='line' id='LC56'><span class="cm"> *          no need to explicitly initialize the driver.</span></div><div class='line' id='LC57'><span class="cm"> *</span></div><div class='line' id='LC58'><span class="cm"> * @init</span></div><div class='line' id='LC59'><span class="cm"> */</span></div><div class='line' id='LC60'><span class="kt">void</span> <span class="nf">dacInit</span><span class="p">(</span><span class="kt">void</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC61'><br/></div><div class='line' id='LC62'>&nbsp;&nbsp;<span class="n">dac_lld_init</span><span class="p">();</span></div><div class='line' id='LC63'><span class="p">}</span></div><div class='line' id='LC64'><br/></div><div class='line' id='LC65'><span class="cm">/**</span></div><div class='line' id='LC66'><span class="cm"> * @brief   Initializes the standard part of a @p DACDriver structure.</span></div><div class='line' id='LC67'><span class="cm"> *</span></div><div class='line' id='LC68'><span class="cm"> * @param[out] dacp     pointer to the @p DACDriver object</span></div><div class='line' id='LC69'><span class="cm"> *</span></div><div class='line' id='LC70'><span class="cm"> * @init</span></div><div class='line' id='LC71'><span class="cm"> */</span></div><div class='line' id='LC72'><span class="kt">void</span> <span class="nf">dacObjectInit</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC73'><br/></div><div class='line' id='LC74'>&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">=</span> <span class="n">DAC_STOP</span><span class="p">;</span></div><div class='line' id='LC75'>&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">config</span> <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC76'><span class="cp">#if DAC_USE_WAIT</span></div><div class='line' id='LC77'>&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="kr">thread</span> <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC78'><span class="cp">#endif </span><span class="cm">/* DAC_USE_WAIT */</span><span class="cp"></span></div><div class='line' id='LC79'><span class="cp">#if DAC_USE_MUTUAL_EXCLUSION</span></div><div class='line' id='LC80'>&nbsp;&nbsp;<span class="n">osalMutexObjectInit</span><span class="p">(</span><span class="o">&amp;</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">mutex</span><span class="p">);</span></div><div class='line' id='LC81'><span class="cp">#endif </span><span class="cm">/* DAC_USE_MUTUAL_EXCLUSION */</span><span class="cp"></span></div><div class='line' id='LC82'><span class="cp">#if defined(DAC_DRIVER_EXT_INIT_HOOK)</span></div><div class='line' id='LC83'>&nbsp;&nbsp;<span class="n">DAC_DRIVER_EXT_INIT_HOOK</span><span class="p">(</span><span class="n">dacp</span><span class="p">);</span></div><div class='line' id='LC84'><span class="cp">#endif</span></div><div class='line' id='LC85'><span class="p">}</span></div><div class='line' id='LC86'><br/></div><div class='line' id='LC87'><span class="cm">/**</span></div><div class='line' id='LC88'><span class="cm"> * @brief   Configures and activates the DAC peripheral.</span></div><div class='line' id='LC89'><span class="cm"> *</span></div><div class='line' id='LC90'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC91'><span class="cm"> * @param[in] config    pointer to the @p DACConfig object</span></div><div class='line' id='LC92'><span class="cm"> *</span></div><div class='line' id='LC93'><span class="cm"> * @api</span></div><div class='line' id='LC94'><span class="cm"> */</span></div><div class='line' id='LC95'><span class="kt">void</span> <span class="nf">dacStart</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">,</span> <span class="k">const</span> <span class="n">DACConfig</span> <span class="o">*</span><span class="n">config</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC96'><br/></div><div class='line' id='LC97'>&nbsp;&nbsp;<span class="n">osalDbgCheck</span><span class="p">((</span><span class="n">dacp</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">)</span> <span class="o">&amp;&amp;</span> <span class="p">(</span><span class="n">config</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">));</span></div><div class='line' id='LC98'><br/></div><div class='line' id='LC99'>&nbsp;&nbsp;<span class="n">osalSysLock</span><span class="p">();</span></div><div class='line' id='LC100'>&nbsp;&nbsp;<span class="n">osalDbgAssert</span><span class="p">((</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_STOP</span><span class="p">)</span> <span class="o">||</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_READY</span><span class="p">),</span></div><div class='line' id='LC101'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="s">&quot;invalid state&quot;</span><span class="p">);</span></div><div class='line' id='LC102'>&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">config</span> <span class="o">=</span> <span class="n">config</span><span class="p">;</span></div><div class='line' id='LC103'>&nbsp;&nbsp;<span class="n">dac_lld_start</span><span class="p">(</span><span class="n">dacp</span><span class="p">);</span></div><div class='line' id='LC104'>&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">=</span> <span class="n">DAC_READY</span><span class="p">;</span></div><div class='line' id='LC105'>&nbsp;&nbsp;<span class="n">osalSysUnlock</span><span class="p">();</span></div><div class='line' id='LC106'><span class="p">}</span></div><div class='line' id='LC107'><br/></div><div class='line' id='LC108'><span class="cm">/**</span></div><div class='line' id='LC109'><span class="cm"> * @brief Deactivates the DAC peripheral.</span></div><div class='line' id='LC110'><span class="cm"> * @note  Deactivating the peripheral also enforces a release of the slave</span></div><div class='line' id='LC111'><span class="cm"> *        select line.</span></div><div class='line' id='LC112'><span class="cm"> *</span></div><div class='line' id='LC113'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC114'><span class="cm"> *</span></div><div class='line' id='LC115'><span class="cm"> * @api</span></div><div class='line' id='LC116'><span class="cm"> */</span></div><div class='line' id='LC117'><span class="kt">void</span> <span class="nf">dacStop</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC118'><br/></div><div class='line' id='LC119'>&nbsp;&nbsp;<span class="n">osalDbgCheck</span><span class="p">(</span><span class="n">dacp</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC120'><br/></div><div class='line' id='LC121'>&nbsp;&nbsp;<span class="n">osalSysLock</span><span class="p">();</span></div><div class='line' id='LC122'>&nbsp;&nbsp;<span class="n">osalDbgAssert</span><span class="p">((</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_STOP</span><span class="p">)</span> <span class="o">||</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_READY</span><span class="p">),</span></div><div class='line' id='LC123'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="s">&quot;invalid state&quot;</span><span class="p">);</span></div><div class='line' id='LC124'>&nbsp;&nbsp;<span class="n">dac_lld_stop</span><span class="p">(</span><span class="n">dacp</span><span class="p">);</span></div><div class='line' id='LC125'>&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">=</span> <span class="n">DAC_STOP</span><span class="p">;</span></div><div class='line' id='LC126'>&nbsp;&nbsp;<span class="n">osalSysUnlock</span><span class="p">();</span></div><div class='line' id='LC127'><span class="p">}</span></div><div class='line' id='LC128'><br/></div><div class='line' id='LC129'><span class="cm">/**</span></div><div class='line' id='LC130'><span class="cm"> * @brief   Starts a DAC conversion.</span></div><div class='line' id='LC131'><span class="cm"> * @details Starts an asynchronous conversion operation.</span></div><div class='line' id='LC132'><span class="cm"> * @note    The buffer is organized as a matrix of M*N elements where M is the</span></div><div class='line' id='LC133'><span class="cm"> *          channels number configured into the conversion group and N is the</span></div><div class='line' id='LC134'><span class="cm"> *          buffer depth. The samples are sequentially written into the buffer</span></div><div class='line' id='LC135'><span class="cm"> *          with no gaps.</span></div><div class='line' id='LC136'><span class="cm"> *</span></div><div class='line' id='LC137'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC138'><span class="cm"> * @param[in] grpp      pointer to a @p DACConversionGroup object</span></div><div class='line' id='LC139'><span class="cm"> * @param[in] samples   pointer to the samples buffer</span></div><div class='line' id='LC140'><span class="cm"> * @param[in] depth     buffer depth (matrix rows number). The buffer depth</span></div><div class='line' id='LC141'><span class="cm"> *                      must be one or an even number.</span></div><div class='line' id='LC142'><span class="cm"> *</span></div><div class='line' id='LC143'><span class="cm"> * @api</span></div><div class='line' id='LC144'><span class="cm"> */</span></div><div class='line' id='LC145'><span class="kt">void</span> <span class="nf">dacStartConversion</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">,</span></div><div class='line' id='LC146'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">const</span> <span class="n">DACConversionGroup</span> <span class="o">*</span><span class="n">grpp</span><span class="p">,</span></div><div class='line' id='LC147'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">const</span> <span class="kt">dacsample_t</span> <span class="o">*</span><span class="n">samples</span><span class="p">,</span></div><div class='line' id='LC148'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">size_t</span> <span class="n">depth</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC149'><br/></div><div class='line' id='LC150'>&nbsp;&nbsp;<span class="n">osalSysLock</span><span class="p">();</span></div><div class='line' id='LC151'>&nbsp;&nbsp;<span class="n">dacStartConversionI</span><span class="p">(</span><span class="n">dacp</span><span class="p">,</span> <span class="n">grpp</span><span class="p">,</span> <span class="n">samples</span><span class="p">,</span> <span class="n">depth</span><span class="p">);</span></div><div class='line' id='LC152'>&nbsp;&nbsp;<span class="n">osalSysUnlock</span><span class="p">();</span></div><div class='line' id='LC153'><span class="p">}</span></div><div class='line' id='LC154'><br/></div><div class='line' id='LC155'><span class="cm">/**</span></div><div class='line' id='LC156'><span class="cm"> * @brief   Starts a DAC conversion.</span></div><div class='line' id='LC157'><span class="cm"> * @details Starts an asynchronous conversion operation.</span></div><div class='line' id='LC158'><span class="cm"> * @post    The callbacks associated to the conversion group will be invoked</span></div><div class='line' id='LC159'><span class="cm"> *          on buffer fill and error events.</span></div><div class='line' id='LC160'><span class="cm"> * @note    The buffer is organized as a matrix of M*N elements where M is the</span></div><div class='line' id='LC161'><span class="cm"> *          channels number configured into the conversion group and N is the</span></div><div class='line' id='LC162'><span class="cm"> *          buffer depth. The samples are sequentially written into the buffer</span></div><div class='line' id='LC163'><span class="cm"> *          with no gaps.</span></div><div class='line' id='LC164'><span class="cm"> *</span></div><div class='line' id='LC165'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC166'><span class="cm"> * @param[in] grpp      pointer to a @p DACConversionGroup object</span></div><div class='line' id='LC167'><span class="cm"> * @param[in] samples   pointer to the samples buffer</span></div><div class='line' id='LC168'><span class="cm"> * @param[in] depth     buffer depth (matrix rows number). The buffer depth</span></div><div class='line' id='LC169'><span class="cm"> *                      must be one or an even number.</span></div><div class='line' id='LC170'><span class="cm"> *</span></div><div class='line' id='LC171'><span class="cm"> * @iclass</span></div><div class='line' id='LC172'><span class="cm"> */</span></div><div class='line' id='LC173'><span class="kt">void</span> <span class="nf">dacStartConversionI</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">,</span></div><div class='line' id='LC174'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">const</span> <span class="n">DACConversionGroup</span> <span class="o">*</span><span class="n">grpp</span><span class="p">,</span></div><div class='line' id='LC175'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">const</span> <span class="kt">dacsample_t</span> <span class="o">*</span><span class="n">samples</span><span class="p">,</span></div><div class='line' id='LC176'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">size_t</span> <span class="n">depth</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC177'><br/></div><div class='line' id='LC178'>&nbsp;&nbsp;<span class="n">osalDbgCheckClassI</span><span class="p">();</span></div><div class='line' id='LC179'>&nbsp;&nbsp;<span class="n">osalDbgCheck</span><span class="p">((</span><span class="n">dacp</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">)</span> <span class="o">&amp;&amp;</span> <span class="p">(</span><span class="n">grpp</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">)</span> <span class="o">&amp;&amp;</span> <span class="p">(</span><span class="n">samples</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">)</span> <span class="o">&amp;&amp;</span></div><div class='line' id='LC180'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">((</span><span class="n">depth</span> <span class="o">==</span> <span class="mi">1</span><span class="p">)</span> <span class="o">||</span> <span class="p">((</span><span class="n">depth</span> <span class="o">&amp;</span> <span class="mi">1</span><span class="p">)</span> <span class="o">==</span> <span class="mi">0</span><span class="p">)));</span></div><div class='line' id='LC181'>&nbsp;&nbsp;<span class="n">osalDbgAssert</span><span class="p">((</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_READY</span><span class="p">)</span> <span class="o">||</span></div><div class='line' id='LC182'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_COMPLETE</span><span class="p">)</span> <span class="o">||</span></div><div class='line' id='LC183'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_ERROR</span><span class="p">),</span></div><div class='line' id='LC184'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="s">&quot;not ready&quot;</span><span class="p">);</span></div><div class='line' id='LC185'><br/></div><div class='line' id='LC186'>&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">samples</span>  <span class="o">=</span> <span class="n">samples</span><span class="p">;</span></div><div class='line' id='LC187'>&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">depth</span>    <span class="o">=</span> <span class="n">depth</span><span class="p">;</span></div><div class='line' id='LC188'>&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">grpp</span>     <span class="o">=</span> <span class="n">grpp</span><span class="p">;</span></div><div class='line' id='LC189'>&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span>    <span class="o">=</span> <span class="n">DAC_ACTIVE</span><span class="p">;</span></div><div class='line' id='LC190'>&nbsp;&nbsp;<span class="n">dac_lld_start_conversion</span><span class="p">(</span><span class="n">dacp</span><span class="p">);</span></div><div class='line' id='LC191'><span class="p">}</span></div><div class='line' id='LC192'><br/></div><div class='line' id='LC193'><span class="cm">/**</span></div><div class='line' id='LC194'><span class="cm"> * @brief   Stops an ongoing conversion.</span></div><div class='line' id='LC195'><span class="cm"> * @details This function stops the currently ongoing conversion and returns</span></div><div class='line' id='LC196'><span class="cm"> *          the driver in the @p DAC_READY state. If there was no conversion</span></div><div class='line' id='LC197'><span class="cm"> *          being processed then the function does nothing.</span></div><div class='line' id='LC198'><span class="cm"> *</span></div><div class='line' id='LC199'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC200'><span class="cm"> *</span></div><div class='line' id='LC201'><span class="cm"> * @api</span></div><div class='line' id='LC202'><span class="cm"> */</span></div><div class='line' id='LC203'><span class="kt">void</span> <span class="nf">dacStopConversion</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC204'><br/></div><div class='line' id='LC205'>&nbsp;&nbsp;<span class="n">osalDbgCheck</span><span class="p">(</span><span class="n">dacp</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC206'><br/></div><div class='line' id='LC207'>&nbsp;&nbsp;<span class="n">osalSysLock</span><span class="p">();</span></div><div class='line' id='LC208'>&nbsp;&nbsp;<span class="n">osalDbgAssert</span><span class="p">((</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_READY</span><span class="p">)</span> <span class="o">||</span></div><div class='line' id='LC209'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_ACTIVE</span><span class="p">),</span></div><div class='line' id='LC210'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="s">&quot;invalid state&quot;</span><span class="p">);</span></div><div class='line' id='LC211'>&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">!=</span> <span class="n">DAC_READY</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC212'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dac_lld_stop_conversion</span><span class="p">(</span><span class="n">dacp</span><span class="p">);</span></div><div class='line' id='LC213'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">grpp</span>  <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC214'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">=</span> <span class="n">DAC_READY</span><span class="p">;</span></div><div class='line' id='LC215'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">_dac_reset_s</span><span class="p">(</span><span class="n">dacp</span><span class="p">);</span></div><div class='line' id='LC216'>&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC217'>&nbsp;&nbsp;<span class="n">osalSysUnlock</span><span class="p">();</span></div><div class='line' id='LC218'><span class="p">}</span></div><div class='line' id='LC219'><br/></div><div class='line' id='LC220'><span class="cm">/**</span></div><div class='line' id='LC221'><span class="cm"> * @brief   Stops an ongoing conversion.</span></div><div class='line' id='LC222'><span class="cm"> * @details This function stops the currently ongoing conversion and returns</span></div><div class='line' id='LC223'><span class="cm"> *          the driver in the @p DAC_READY state. If there was no conversion</span></div><div class='line' id='LC224'><span class="cm"> *          being processed then the function does nothing.</span></div><div class='line' id='LC225'><span class="cm"> *</span></div><div class='line' id='LC226'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC227'><span class="cm"> *</span></div><div class='line' id='LC228'><span class="cm"> * @iclass</span></div><div class='line' id='LC229'><span class="cm"> */</span></div><div class='line' id='LC230'><span class="kt">void</span> <span class="nf">dacStopConversionI</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC231'><br/></div><div class='line' id='LC232'>&nbsp;&nbsp;<span class="n">osalDbgCheckClassI</span><span class="p">();</span></div><div class='line' id='LC233'>&nbsp;&nbsp;<span class="n">osalDbgCheck</span><span class="p">(</span><span class="n">dacp</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC234'>&nbsp;&nbsp;<span class="n">osalDbgAssert</span><span class="p">((</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_READY</span><span class="p">)</span> <span class="o">||</span></div><div class='line' id='LC235'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_ACTIVE</span><span class="p">)</span> <span class="o">||</span></div><div class='line' id='LC236'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_COMPLETE</span><span class="p">),</span></div><div class='line' id='LC237'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="s">&quot;invalid state&quot;</span><span class="p">);</span></div><div class='line' id='LC238'><br/></div><div class='line' id='LC239'>&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">!=</span> <span class="n">DAC_READY</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC240'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dac_lld_stop_conversion</span><span class="p">(</span><span class="n">dacp</span><span class="p">);</span></div><div class='line' id='LC241'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">grpp</span>  <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC242'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">=</span> <span class="n">DAC_READY</span><span class="p">;</span></div><div class='line' id='LC243'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">_dac_reset_i</span><span class="p">(</span><span class="n">dacp</span><span class="p">);</span></div><div class='line' id='LC244'>&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC245'><span class="p">}</span></div><div class='line' id='LC246'><br/></div><div class='line' id='LC247'><span class="cp">#if DAC_USE_WAIT || defined(__DOXYGEN__)</span></div><div class='line' id='LC248'><span class="cm">/**</span></div><div class='line' id='LC249'><span class="cm"> * @brief   Performs a DAC conversion.</span></div><div class='line' id='LC250'><span class="cm"> * @details Performs a synchronous conversion operation.</span></div><div class='line' id='LC251'><span class="cm"> * @note    The buffer is organized as a matrix of M*N elements where M is the</span></div><div class='line' id='LC252'><span class="cm"> *          channels number configured into the conversion group and N is the</span></div><div class='line' id='LC253'><span class="cm"> *          buffer depth. The samples are sequentially written into the buffer</span></div><div class='line' id='LC254'><span class="cm"> *          with no gaps.</span></div><div class='line' id='LC255'><span class="cm"> *</span></div><div class='line' id='LC256'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC257'><span class="cm"> * @param[in] grpp      pointer to a @p DACConversionGroup object</span></div><div class='line' id='LC258'><span class="cm"> * @param[out] samples  pointer to the samples buffer</span></div><div class='line' id='LC259'><span class="cm"> * @param[in] depth     buffer depth (matrix rows number). The buffer depth</span></div><div class='line' id='LC260'><span class="cm"> *                      must be one or an even number.</span></div><div class='line' id='LC261'><span class="cm"> * @return              The operation result.</span></div><div class='line' id='LC262'><span class="cm"> * @retval RDY_OK       Conversion finished.</span></div><div class='line' id='LC263'><span class="cm"> * @retval RDY_RESET    The conversion has been stopped using</span></div><div class='line' id='LC264'><span class="cm"> *                      @p acdStopConversion() or @p acdStopConversionI(),</span></div><div class='line' id='LC265'><span class="cm"> *                      the result buffer may contain incorrect data.</span></div><div class='line' id='LC266'><span class="cm"> * @retval RDY_TIMEOUT  The conversion has been stopped because an hardware</span></div><div class='line' id='LC267'><span class="cm"> *                      error.</span></div><div class='line' id='LC268'><span class="cm"> *</span></div><div class='line' id='LC269'><span class="cm"> * @api</span></div><div class='line' id='LC270'><span class="cm"> */</span></div><div class='line' id='LC271'><span class="kt">msg_t</span> <span class="nf">dacConvert</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">,</span></div><div class='line' id='LC272'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">const</span> <span class="n">DACConversionGroup</span> <span class="o">*</span><span class="n">grpp</span><span class="p">,</span></div><div class='line' id='LC273'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">const</span> <span class="kt">dacsample_t</span> <span class="o">*</span><span class="n">samples</span><span class="p">,</span></div><div class='line' id='LC274'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">size_t</span> <span class="n">depth</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC275'>&nbsp;&nbsp;<span class="kt">msg_t</span> <span class="n">msg</span><span class="p">;</span></div><div class='line' id='LC276'><br/></div><div class='line' id='LC277'>&nbsp;&nbsp;<span class="n">osalSysLock</span><span class="p">();</span></div><div class='line' id='LC278'>&nbsp;&nbsp;<span class="n">osalDbgAssert</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="kr">thread</span> <span class="o">==</span> <span class="nb">NULL</span><span class="p">,</span> <span class="s">&quot;already waiting&quot;</span><span class="p">);</span></div><div class='line' id='LC279'>&nbsp;&nbsp;<span class="n">dacStartConversionI</span><span class="p">(</span><span class="n">dacp</span><span class="p">,</span> <span class="n">grpp</span><span class="p">,</span> <span class="n">samples</span><span class="p">,</span> <span class="n">depth</span><span class="p">);</span></div><div class='line' id='LC280'>&nbsp;&nbsp;<span class="n">msg</span> <span class="o">=</span> <span class="n">osalThreadSuspendS</span><span class="p">(</span><span class="o">&amp;</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="kr">thread</span><span class="p">);</span></div><div class='line' id='LC281'>&nbsp;&nbsp;<span class="n">osalSysUnlock</span><span class="p">();</span></div><div class='line' id='LC282'>&nbsp;&nbsp;<span class="k">return</span> <span class="n">msg</span><span class="p">;</span></div><div class='line' id='LC283'><span class="p">}</span></div><div class='line' id='LC284'><span class="cp">#endif </span><span class="cm">/* DAC_USE_WAIT */</span><span class="cp"></span></div><div class='line' id='LC285'><br/></div><div class='line' id='LC286'><span class="cp">#if DAC_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)</span></div><div class='line' id='LC287'><span class="cm">/**</span></div><div class='line' id='LC288'><span class="cm"> * @brief   Gains exclusive access to the DAC bus.</span></div><div class='line' id='LC289'><span class="cm"> * @details This function tries to gain ownership to the DAC bus, if the bus</span></div><div class='line' id='LC290'><span class="cm"> *          is already being used then the invoking thread is queued.</span></div><div class='line' id='LC291'><span class="cm"> * @pre     In order to use this function the option @p DAC_USE_MUTUAL_EXCLUSION</span></div><div class='line' id='LC292'><span class="cm"> *          must be enabled.</span></div><div class='line' id='LC293'><span class="cm"> *</span></div><div class='line' id='LC294'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC295'><span class="cm"> *</span></div><div class='line' id='LC296'><span class="cm"> * @api</span></div><div class='line' id='LC297'><span class="cm"> */</span></div><div class='line' id='LC298'><span class="kt">void</span> <span class="nf">dacAcquireBus</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC299'><br/></div><div class='line' id='LC300'>&nbsp;&nbsp;<span class="n">osalDbgCheck</span><span class="p">(</span><span class="n">dacp</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC301'><br/></div><div class='line' id='LC302'>&nbsp;&nbsp;<span class="n">osalMutexLock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">mutex</span><span class="p">);</span></div><div class='line' id='LC303'><span class="p">}</span></div><div class='line' id='LC304'><br/></div><div class='line' id='LC305'><span class="cm">/**</span></div><div class='line' id='LC306'><span class="cm"> * @brief   Releases exclusive access to the DAC bus.</span></div><div class='line' id='LC307'><span class="cm"> * @pre     In order to use this function the option @p DAC_USE_MUTUAL_EXCLUSION</span></div><div class='line' id='LC308'><span class="cm"> *          must be enabled.</span></div><div class='line' id='LC309'><span class="cm"> *</span></div><div class='line' id='LC310'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC311'><span class="cm"> *</span></div><div class='line' id='LC312'><span class="cm"> * @api</span></div><div class='line' id='LC313'><span class="cm"> */</span></div><div class='line' id='LC314'><span class="kt">void</span> <span class="nf">dacReleaseBus</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC315'><br/></div><div class='line' id='LC316'>&nbsp;&nbsp;<span class="n">osalDbgCheck</span><span class="p">(</span><span class="n">dacp</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC317'><br/></div><div class='line' id='LC318'>&nbsp;&nbsp;<span class="n">osalMutexUnlock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">mutex</span><span class="p">);</span></div><div class='line' id='LC319'><span class="p">}</span></div><div class='line' id='LC320'><span class="cp">#endif </span><span class="cm">/* DAC_USE_MUTUAL_EXCLUSION */</span><span class="cp"></span></div><div class='line' id='LC321'><br/></div><div class='line' id='LC322'><span class="cp">#endif </span><span class="cm">/* HAL_USE_DAC */</span><span class="cp"></span></div><div class='line' id='LC323'><br/></div><div class='line' id='LC324'><span class="cm">/** @} */</span></div></pre></div></td>
          </tr>
        </table>
  </div>

  </div>
</div>

<a href="#jump-to-line" rel="facebox[.linejump]" data-hotkey="l" class="js-jump-to-line" style="display:none">Jump to Line</a>
<div id="jump-to-line" style="display:none">
  <form accept-charset="UTF-8" class="js-jump-to-line-form">
    <input class="linejump-input js-jump-to-line-field" type="text" placeholder="Jump to line&hellip;" autofocus>
    <button type="submit" class="button">Go</button>
  </form>
</div>

        </div>

      </div><!-- /.repo-container -->
      <div class="modal-backdrop"></div>
    </div><!-- /.container -->
  </div><!-- /.site -->


    </div><!-- /.wrapper -->

      <div class="container">
  <div class="site-footer">
    <ul class="site-footer-links right">
      <li><a href="https://status.github.com/">Status</a></li>
      <li><a href="http://developer.github.com">API</a></li>
      <li><a href="http://training.github.com">Training</a></li>
      <li><a href="http://shop.github.com">Shop</a></li>
      <li><a href="/blog">Blog</a></li>
      <li><a href="/about">About</a></li>

    </ul>

    <a href="/">
      <span class="mega-octicon octicon-mark-github" title="GitHub"></span>
    </a>

    <ul class="site-footer-links">
      <li>&copy; 2014 <span title="0.07444s from github-fe122-cp1-prd.iad.github.net">GitHub</span>, Inc.</li>
        <li><a href="/site/terms">Terms</a></li>
        <li><a href="/site/privacy">Privacy</a></li>
        <li><a href="/security">Security</a></li>
        <li><a href="/contact">Contact</a></li>
    </ul>
  </div><!-- /.site-footer -->
</div><!-- /.container -->


    <div class="fullscreen-overlay js-fullscreen-overlay" id="fullscreen_overlay">
  <div class="fullscreen-container js-fullscreen-container">
    <div class="textarea-wrap">
      <textarea name="fullscreen-contents" id="fullscreen-contents" class="js-fullscreen-contents" placeholder="" data-suggester="fullscreen_suggester"></textarea>
    </div>
  </div>
  <div class="fullscreen-sidebar">
    <a href="#" class="exit-fullscreen js-exit-fullscreen tooltipped tooltipped-w" aria-label="Exit Zen Mode">
      <span class="mega-octicon octicon-screen-normal"></span>
    </a>
    <a href="#" class="theme-switcher js-theme-switcher tooltipped tooltipped-w"
      aria-label="Switch themes">
      <span class="octicon octicon-color-mode"></span>
    </a>
  </div>
</div>



    <div id="ajax-error-message" class="flash flash-error">
      <span class="octicon octicon-alert"></span>
      <a href="#" class="octicon octicon-remove-close close js-ajax-error-dismiss"></a>
      Something went wrong with that request. Please try again.
    </div>

  </body>
</html>

