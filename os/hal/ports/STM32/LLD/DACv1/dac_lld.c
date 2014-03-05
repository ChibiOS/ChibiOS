




<!DOCTYPE html>
<html>
  <head prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb# object: http://ogp.me/ns/object# article: http://ogp.me/ns/article# profile: http://ogp.me/ns/profile#">
    <meta charset='utf-8'>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title>ChibiOS/os/hal/ports/STM32/DACv1/dac_lld.c at dac-3.0 · mobyfab/ChibiOS · GitHub</title>
    <link rel="search" type="application/opensearchdescription+xml" href="/opensearch.xml" title="GitHub" />
    <link rel="fluid-icon" href="https://github.com/fluidicon.png" title="GitHub" />
    <link rel="apple-touch-icon" sizes="57x57" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="114x114" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="72x72" href="/apple-touch-icon-144.png" />
    <link rel="apple-touch-icon" sizes="144x144" href="/apple-touch-icon-144.png" />
    <meta property="fb:app_id" content="1401488693436528"/>

      <meta content="@github" name="twitter:site" /><meta content="summary" name="twitter:card" /><meta content="mobyfab/ChibiOS" name="twitter:title" /><meta content="Mirror of the ChibiOS SVN repository (updated every 10 minutes)" name="twitter:description" /><meta content="https://1.gravatar.com/avatar/f152cebdb9ab085fd2369e2e9eb7342b?d=https%3A%2F%2Fidenticons.github.com%2Fdecfe48600b86598c97bc3499607b4a9.png&amp;r=x&amp;s=400" name="twitter:image:src" />
<meta content="GitHub" property="og:site_name" /><meta content="object" property="og:type" /><meta content="https://1.gravatar.com/avatar/f152cebdb9ab085fd2369e2e9eb7342b?d=https%3A%2F%2Fidenticons.github.com%2Fdecfe48600b86598c97bc3499607b4a9.png&amp;r=x&amp;s=400" property="og:image" /><meta content="mobyfab/ChibiOS" property="og:title" /><meta content="https://github.com/mobyfab/ChibiOS" property="og:url" /><meta content="Mirror of the ChibiOS SVN repository (updated every 10 minutes)" property="og:description" />

    <meta name="hostname" content="github-fe132-cp1-prd.iad.github.net">
    <meta name="ruby" content="ruby 2.1.0p0-github-tcmalloc (87c9373a41) [x86_64-linux]">
    <link rel="assets" href="https://github.global.ssl.fastly.net/">
    <link rel="conduit-xhr" href="https://ghconduit.com:25035/">
    <link rel="xhr-socket" href="/_sockets" />


    <meta name="msapplication-TileImage" content="/windows-tile.png" />
    <meta name="msapplication-TileColor" content="#ffffff" />
    <meta name="selected-link" value="repo_source" data-pjax-transient />
    <meta content="collector.githubapp.com" name="octolytics-host" /><meta content="collector-cdn.github.com" name="octolytics-script-host" /><meta content="github" name="octolytics-app-id" /><meta content="A4810129:1E8A:132FF1B:5316FA02" name="octolytics-dimension-request_id" />
    

    
    
    <link rel="icon" type="image/x-icon" href="/favicon.ico" />

    <meta content="authenticity_token" name="csrf-param" />
<meta content="+I75OT9V9FDjod/MtXvKnsFPyB9rwHLiS1IyZTd0pHk=" name="csrf-token" />

    <link href="https://github.global.ssl.fastly.net/assets/github-144f6fbd93cc11df8cc03bc000351f75fa1e5875.css" media="all" rel="stylesheet" type="text/css" />
    <link href="https://github.global.ssl.fastly.net/assets/github2-ff993fcf465373a314708ecedfbbffc29250b930.css" media="all" rel="stylesheet" type="text/css" />
    
    


      <script crossorigin="anonymous" src="https://github.global.ssl.fastly.net/assets/frameworks-490867a148cf94468c141093ccd4e56ed2144eb9.js" type="text/javascript"></script>
      <script async="async" crossorigin="anonymous" src="https://github.global.ssl.fastly.net/assets/github-a74d8739a439667125da3a5c5435962d57986a54.js" type="text/javascript"></script>
      
      <meta http-equiv="x-pjax-version" content="348cbaf0429fd819da9310a1f5148dfb">

        <link data-pjax-transient rel='permalink' href='/mobyfab/ChibiOS/blob/42c482fbf2b4feae8dd524e664d084bb6de4f9ba/os/hal/ports/STM32/DACv1/dac_lld.c'>

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
      <a class="button signin" href="/login?return_to=%2Fmobyfab%2FChibiOS%2Fblob%2Fdac-3.0%2Fos%2Fhal%2Fports%2FSTM32%2FDACv1%2Fdac_lld.c">Sign in</a>
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
          


<!-- blob contrib key: blob_contributors:v21:6407bec8a3dfbe0cebc1290326a6cf21 -->

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
              <a href="/mobyfab/ChibiOS/blob/chfs_test/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="chfs_test"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="chfs_test">chfs_test</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/coldfire_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="coldfire_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="coldfire_dev">coldfire_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/condvars_test/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="condvars_test"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="condvars_test">condvars_test</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/condvars_test_alt/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="condvars_test_alt"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="condvars_test_alt">condvars_test_alt</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/cpp_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="cpp_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="cpp_dev">cpp_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/dac/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="dac"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="dac">dac</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item selected">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/dac-3.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="dac-3.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="dac-3.0">dac-3.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/dac-new/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="dac-new"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="dac-new">dac-new</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/dynclk_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="dynclk_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="dynclk_dev">dynclk_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/fujitsu_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="fujitsu_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="fujitsu_dev">fujitsu_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/fujitsu_dev_2.5.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="fujitsu_dev_2.5.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="fujitsu_dev_2.5.0">fujitsu_dev_2.5.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/h8s__dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="h8s__dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="h8s__dev">h8s__dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/h8s_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="h8s_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="h8s_dev">h8s_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/i2c_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="i2c_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="i2c_dev">i2c_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/kernopt_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="kernopt_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="kernopt_dev">kernopt_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/lwip_test/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="lwip_test"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="lwip_test">lwip_test</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/master/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="master"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="master">master</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/nil_trunk/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="nil_trunk"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="nil_trunk">nil_trunk</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/origin/master/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="origin/master"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="origin/master">origin/master</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/portable_hal_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="portable_hal_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="portable_hal_dev">portable_hal_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/ppc_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="ppc_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="ppc_dev">ppc_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/pvd2_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="pvd2_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="pvd2_dev">pvd2_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/pvd_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="pvd_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="pvd_dev">pvd_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/rtc_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="rtc_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="rtc_dev">rtc_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/sdc_dev2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="sdc_dev2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="sdc_dev2">sdc_dev2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/sdio_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="sdio_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="sdio_dev">sdio_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/spc5studio_components_trunk/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="spc5studio_components_trunk"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="spc5studio_components_trunk">spc5studio_components_trunk</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_1.0.x/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stable_1.0.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_1.0.x">stable_1.0.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_1.2.x/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stable_1.2.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_1.2.x">stable_1.2.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_1.4.x/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stable_1.4.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_1.4.x">stable_1.4.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_2.0.x/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stable_2.0.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_2.0.x">stable_2.0.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_2.2.x/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stable_2.2.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_2.2.x">stable_2.2.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_2.4.x/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stable_2.4.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_2.4.x">stable_2.4.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stable_2.6.x/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stable_2.6.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_2.6.x">stable_2.6.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stm8_cosmic_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stm8_cosmic_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stm8_cosmic_dev">stm8_cosmic_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stm8_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stm8_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stm8_dev">stm8_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stm8_hal/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stm8_hal"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stm8_hal">stm8_hal</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stm32_ethernet_test/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stm32_ethernet_test"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stm32_ethernet_test">stm32_ethernet_test</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stm32_ethernet_wrapper/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stm32_ethernet_wrapper"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stm32_ethernet_wrapper">stm32_ethernet_wrapper</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/stm32f2xx_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="stm32f2xx_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stm32f2xx_dev">stm32f2xx_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/nil_0.0.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/nil_0.0.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/nil_0.0.1">tags/nil_0.0.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/spc5studio_components_1.1.0.RC1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/spc5studio_components_1.1.0.RC1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/spc5studio_components_1.1.0.RC1">tags/spc5studio_components_1.1.0.RC1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/spc5studio_components_frozen_1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/spc5studio_components_frozen_1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/spc5studio_components_frozen_1">tags/spc5studio_components_frozen_1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.1.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.1.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.1.0">tags/ver_0.1.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.1.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.1.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.1.1">tags/ver_0.1.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.2.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.2.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.2.0">tags/ver_0.2.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.2.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.2.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.2.1">tags/ver_0.2.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.3.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.0">tags/ver_0.3.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.3.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.1">tags/ver_0.3.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.3.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.2">tags/ver_0.3.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.3/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.3.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.3">tags/ver_0.3.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.4/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.3.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.4">tags/ver_0.3.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.5/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.3.5"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.5">tags/ver_0.3.5</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.3.6/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.3.6"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.3.6">tags/ver_0.3.6</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.4.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.4.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.4.0">tags/ver_0.4.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.4.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.4.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.4.1">tags/ver_0.4.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.4.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.4.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.4.2">tags/ver_0.4.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.4.4/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.4.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.4.4">tags/ver_0.4.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.4.5/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.4.5"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.4.5">tags/ver_0.4.5</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.5.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.5.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.5.0">tags/ver_0.5.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.5.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.5.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.5.1">tags/ver_0.5.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.5.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.5.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.5.2">tags/ver_0.5.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.5.3/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.5.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.5.3">tags/ver_0.5.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.5.4/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.5.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.5.4">tags/ver_0.5.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.6.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.0">tags/ver_0.6.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.6.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.1">tags/ver_0.6.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.6.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.2">tags/ver_0.6.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.4/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.6.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.4">tags/ver_0.6.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.5/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.6.5"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.5">tags/ver_0.6.5</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.6/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.6.6"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.6">tags/ver_0.6.6</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.7/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.6.7"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.7">tags/ver_0.6.7</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.8/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.6.8"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.8">tags/ver_0.6.8</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.9/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.6.9"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.9">tags/ver_0.6.9</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.6.10/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.6.10"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.6.10">tags/ver_0.6.10</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.7.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.7.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.7.0">tags/ver_0.7.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.7.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.7.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.7.1">tags/ver_0.7.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.7.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.7.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.7.2">tags/ver_0.7.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.7.3/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.7.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.7.3">tags/ver_0.7.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.8.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.8.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.8.0">tags/ver_0.8.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.8.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.8.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.8.1">tags/ver_0.8.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.8.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.8.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.8.2">tags/ver_0.8.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_0.8.3/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_0.8.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_0.8.3">tags/ver_0.8.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.0.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.0.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.0.0">tags/ver_1.0.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.0.0rc1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.0.0rc1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.0.0rc1">tags/ver_1.0.0rc1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.0.0rc2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.0.0rc2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.0.0rc2">tags/ver_1.0.0rc2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.0.0rc3/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.0.0rc3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.0.0rc3">tags/ver_1.0.0rc3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.0.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.0.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.0.1">tags/ver_1.0.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.0.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.0.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.0.2">tags/ver_1.0.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.1.0unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.1.0unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.1.0unstable">tags/ver_1.1.0unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.1.1unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.1.1unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.1.1unstable">tags/ver_1.1.1unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.1.2unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.1.2unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.1.2unstable">tags/ver_1.1.2unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.1.3unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.1.3unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.1.3unstable">tags/ver_1.1.3unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.2.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.2.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.2.0">tags/ver_1.2.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.2.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.2.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.2.1">tags/ver_1.2.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.2.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.2.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.2.2">tags/ver_1.2.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.2.3/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.2.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.2.3">tags/ver_1.2.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.2.4/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.2.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.2.4">tags/ver_1.2.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.0unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.3.0unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.0unstable">tags/ver_1.3.0unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.1unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.3.1unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.1unstable">tags/ver_1.3.1unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.2temporary/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.3.2temporary"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.2temporary">tags/ver_1.3.2temporary</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.2unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.3.2unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.2unstable">tags/ver_1.3.2unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.3unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.3.3unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.3unstable">tags/ver_1.3.3unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.4unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.3.4unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.4unstable">tags/ver_1.3.4unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.5unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.3.5unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.5unstable">tags/ver_1.3.5unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.6unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.3.6unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.6unstable">tags/ver_1.3.6unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.7unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.3.7unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.7unstable">tags/ver_1.3.7unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.3.8unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.3.8unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.3.8unstable">tags/ver_1.3.8unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.4.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.4.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.4.0">tags/ver_1.4.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.4.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.4.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.4.1">tags/ver_1.4.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.4.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.4.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.4.2">tags/ver_1.4.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.4.3/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.4.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.4.3">tags/ver_1.4.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.0unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.5.0unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.0unstable">tags/ver_1.5.0unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.1unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.5.1unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.1unstable">tags/ver_1.5.1unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.2unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.5.2unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.2unstable">tags/ver_1.5.2unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.3unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.5.3unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.3unstable">tags/ver_1.5.3unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.4unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.5.4unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.4unstable">tags/ver_1.5.4unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.5unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.5.5unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.5unstable">tags/ver_1.5.5unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.6unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.5.6unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.6unstable">tags/ver_1.5.6unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.7unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.5.7unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.7unstable">tags/ver_1.5.7unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.8unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.5.8unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.8unstable">tags/ver_1.5.8unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_1.5.9unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_1.5.9unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_1.5.9unstable">tags/ver_1.5.9unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.0.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.0">tags/ver_2.0.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.0.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.1">tags/ver_2.0.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.0.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.2">tags/ver_2.0.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.3/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.0.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.3">tags/ver_2.0.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.4/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.0.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.4">tags/ver_2.0.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.5/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.0.5"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.5">tags/ver_2.0.5</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.6/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.0.6"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.6">tags/ver_2.0.6</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.7/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.0.7"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.7">tags/ver_2.0.7</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.8/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.0.8"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.8">tags/ver_2.0.8</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.9/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.0.9"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.9">tags/ver_2.0.9</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.0.10/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.0.10"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.0.10">tags/ver_2.0.10</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.0unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.1.0unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.0unstable">tags/ver_2.1.0unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.1unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.1.1unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.1unstable">tags/ver_2.1.1unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.2unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.1.2unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.2unstable">tags/ver_2.1.2unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.3unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.1.3unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.3unstable">tags/ver_2.1.3unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.4unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.1.4unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.4unstable">tags/ver_2.1.4unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.5unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.1.5unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.5unstable">tags/ver_2.1.5unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.6unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.1.6unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.6unstable">tags/ver_2.1.6unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.1.7unstable/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.1.7unstable"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.1.7unstable">tags/ver_2.1.7unstable</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.2.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.0">tags/ver_2.2.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.2.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.1">tags/ver_2.2.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.2.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.2">tags/ver_2.2.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.3/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.2.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.3">tags/ver_2.2.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.4/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.2.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.4">tags/ver_2.2.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.5/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.2.5"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.5">tags/ver_2.2.5</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.6/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.2.6"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.6">tags/ver_2.2.6</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.7/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.2.7"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.7">tags/ver_2.2.7</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.8/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.2.8"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.8">tags/ver_2.2.8</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.2.9/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.2.9"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.2.9">tags/ver_2.2.9</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.3.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.3.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.3.0">tags/ver_2.3.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.3.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.3.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.3.1">tags/ver_2.3.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.3.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.3.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.3.2">tags/ver_2.3.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.3.3/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.3.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.3.3">tags/ver_2.3.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.3.4/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.3.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.3.4">tags/ver_2.3.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.4.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.4.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.4.0">tags/ver_2.4.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.4.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.4.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.4.1">tags/ver_2.4.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.4.2/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.4.2"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.4.2">tags/ver_2.4.2</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.4.3/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.4.3"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.4.3">tags/ver_2.4.3</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.5.0/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.5.0"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.5.0">tags/ver_2.5.0</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tags/ver_2.5.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tags/ver_2.5.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tags/ver_2.5.1">tags/ver_2.5.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/trunk/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="trunk"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="trunk">trunk</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/tsc/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="tsc"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="tsc">tsc</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/unstable_1.1/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="unstable_1.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="unstable_1.1">unstable_1.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/usb/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="usb"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="usb">usb</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/usb_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="usb_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="usb_dev">usb_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/utils_dev/os/hal/ports/STM32/DACv1/dac_lld.c"
                 data-name="utils_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="utils_dev">utils_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/mobyfab/ChibiOS/blob/various/os/hal/ports/STM32/DACv1/dac_lld.c"
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
    <span class='repo-root js-repo-root'><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/mobyfab/ChibiOS/tree/dac-3.0" data-branch="dac-3.0" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">ChibiOS</span></a></span></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/mobyfab/ChibiOS/tree/dac-3.0/os" data-branch="dac-3.0" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">os</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/mobyfab/ChibiOS/tree/dac-3.0/os/hal" data-branch="dac-3.0" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">hal</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/mobyfab/ChibiOS/tree/dac-3.0/os/hal/ports" data-branch="dac-3.0" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">ports</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/mobyfab/ChibiOS/tree/dac-3.0/os/hal/ports/STM32" data-branch="dac-3.0" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">STM32</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/mobyfab/ChibiOS/tree/dac-3.0/os/hal/ports/STM32/DACv1" data-branch="dac-3.0" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">DACv1</span></a></span><span class="separator"> / </span><strong class="final-path">dac_lld.c</strong> <span aria-label="copy to clipboard" class="js-zeroclipboard minibutton zeroclipboard-button" data-clipboard-text="os/hal/ports/STM32/DACv1/dac_lld.c" data-copied-hint="copied!"><span class="octicon octicon-clippy"></span></span>
  </div>
</div>


  <div class="commit commit-loader file-history-tease js-deferred-content" data-url="/mobyfab/ChibiOS/contributors/dac-3.0/os/hal/ports/STM32/DACv1/dac_lld.c">
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
          <span>352 lines (313 sloc)</span>
          <span class="meta-divider"></span>
        <span>12.38 kb</span>
      </div>
      <div class="actions">
        <div class="button-group">
            <a class="minibutton tooltipped tooltipped-w"
               href="http://windows.github.com" aria-label="Open this file in GitHub for Windows">
                <span class="octicon octicon-device-desktop"></span> Open
            </a>
              <a class="minibutton disabled tooltipped tooltipped-w" href="#"
                 aria-label="You must be signed in to make or propose changes">Edit</a>
          <a href="/mobyfab/ChibiOS/raw/dac-3.0/os/hal/ports/STM32/DACv1/dac_lld.c" class="button minibutton " id="raw-url">Raw</a>
            <a href="/mobyfab/ChibiOS/blame/dac-3.0/os/hal/ports/STM32/DACv1/dac_lld.c" class="button minibutton js-update-url-with-hash">Blame</a>
          <a href="/mobyfab/ChibiOS/commits/dac-3.0/os/hal/ports/STM32/DACv1/dac_lld.c" class="button minibutton " rel="nofollow">History</a>
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
<span id="L325" rel="#L325">325</span>
<span id="L326" rel="#L326">326</span>
<span id="L327" rel="#L327">327</span>
<span id="L328" rel="#L328">328</span>
<span id="L329" rel="#L329">329</span>
<span id="L330" rel="#L330">330</span>
<span id="L331" rel="#L331">331</span>
<span id="L332" rel="#L332">332</span>
<span id="L333" rel="#L333">333</span>
<span id="L334" rel="#L334">334</span>
<span id="L335" rel="#L335">335</span>
<span id="L336" rel="#L336">336</span>
<span id="L337" rel="#L337">337</span>
<span id="L338" rel="#L338">338</span>
<span id="L339" rel="#L339">339</span>
<span id="L340" rel="#L340">340</span>
<span id="L341" rel="#L341">341</span>
<span id="L342" rel="#L342">342</span>
<span id="L343" rel="#L343">343</span>
<span id="L344" rel="#L344">344</span>
<span id="L345" rel="#L345">345</span>
<span id="L346" rel="#L346">346</span>
<span id="L347" rel="#L347">347</span>
<span id="L348" rel="#L348">348</span>
<span id="L349" rel="#L349">349</span>
<span id="L350" rel="#L350">350</span>
<span id="L351" rel="#L351">351</span>

            </td>
            <td class="blob-line-code"><div class="code-body highlight"><pre><div class='line' id='LC1'><span class="cm">/*</span></div><div class='line' id='LC2'><span class="cm">    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,</span></div><div class='line' id='LC3'><span class="cm">                 2011,2012 Giovanni Di Sirio.</span></div><div class='line' id='LC4'><br/></div><div class='line' id='LC5'><span class="cm">    This file is part of ChibiOS/RT.</span></div><div class='line' id='LC6'><br/></div><div class='line' id='LC7'><span class="cm">    ChibiOS/RT is free software; you can redistribute it and/or modify</span></div><div class='line' id='LC8'><span class="cm">    it under the terms of the GNU General Public License as published by</span></div><div class='line' id='LC9'><span class="cm">    the Free Software Foundation; either version 3 of the License, or</span></div><div class='line' id='LC10'><span class="cm">    (at your option) any later version.</span></div><div class='line' id='LC11'><br/></div><div class='line' id='LC12'><span class="cm">    ChibiOS/RT is distributed in the hope that it will be useful,</span></div><div class='line' id='LC13'><span class="cm">    but WITHOUT ANY WARRANTY; without even the implied warranty of</span></div><div class='line' id='LC14'><span class="cm">    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the</span></div><div class='line' id='LC15'><span class="cm">    GNU General Public License for more details.</span></div><div class='line' id='LC16'><br/></div><div class='line' id='LC17'><span class="cm">    You should have received a copy of the GNU General Public License</span></div><div class='line' id='LC18'><span class="cm">    along with this program.  If not, see &lt;http://www.gnu.org/licenses/&gt;.</span></div><div class='line' id='LC19'><span class="cm">*/</span></div><div class='line' id='LC20'><br/></div><div class='line' id='LC21'><span class="cm">/**</span></div><div class='line' id='LC22'><span class="cm"> * @file    STM32/DACv1/dac_lld.c</span></div><div class='line' id='LC23'><span class="cm"> * @brief   STM32 DAC subsystem low level driver source.</span></div><div class='line' id='LC24'><span class="cm"> *</span></div><div class='line' id='LC25'><span class="cm"> * @addtogroup DAC</span></div><div class='line' id='LC26'><span class="cm"> * @{</span></div><div class='line' id='LC27'><span class="cm"> */</span></div><div class='line' id='LC28'><br/></div><div class='line' id='LC29'><span class="cp">#include &quot;hal.h&quot;</span></div><div class='line' id='LC30'><br/></div><div class='line' id='LC31'><span class="cp">#if HAL_USE_DAC || defined(__DOXYGEN__)</span></div><div class='line' id='LC32'><br/></div><div class='line' id='LC33'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC34'><span class="cm">/* Driver local definitions.                                                 */</span></div><div class='line' id='LC35'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC36'><br/></div><div class='line' id='LC37'><span class="cp">#if !defined(DAC1)</span></div><div class='line' id='LC38'><span class="cp">#define DAC1 DAC</span></div><div class='line' id='LC39'><span class="cp">#define rccEnableDAC1 rccEnableDAC</span></div><div class='line' id='LC40'><span class="cp">#define rccDisableDAC1 rccDisableDAC</span></div><div class='line' id='LC41'><span class="cp">#endif</span></div><div class='line' id='LC42'><br/></div><div class='line' id='LC43'><span class="cp">#define DAC_CHN1_DMA_CHANNEL                                                 \</span></div><div class='line' id='LC44'><span class="cp">  STM32_DMA_GETCHANNEL(STM32_DAC_CHN1_DMA_STREAM,                            \</span></div><div class='line' id='LC45'><span class="cp">                       STM32_DAC_CHN1_DMA_CHN)</span></div><div class='line' id='LC46'><br/></div><div class='line' id='LC47'><span class="cp">#define DAC_CHN2_DMA_CHANNEL                                                 \</span></div><div class='line' id='LC48'><span class="cp">  STM32_DMA_GETCHANNEL(STM32_DAC_CHN2_DMA_STREAM,                            \</span></div><div class='line' id='LC49'><span class="cp">                       STM32_DAC_CHN2_DMA_CHN)</span></div><div class='line' id='LC50'><span class="cp">           </span></div><div class='line' id='LC51'><span class="cp">#define DAC_CHN3_DMA_CHANNEL                                                 \</span></div><div class='line' id='LC52'><span class="cp">  STM32_DMA_GETCHANNEL(STM32_DAC_CHN3_DMA_STREAM,                            \</span></div><div class='line' id='LC53'><span class="cp">                       STM32_DAC_CHN3_DMA_CHN)</span></div><div class='line' id='LC54'><br/></div><div class='line' id='LC55'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC56'><span class="cm">/* Driver exported variables.                                                */</span></div><div class='line' id='LC57'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC58'><br/></div><div class='line' id='LC59'><span class="cm">/** @brief CHN1 driver identifier.*/</span></div><div class='line' id='LC60'><span class="cp">#if STM32_DAC_USE_CHN1 || defined(__DOXYGEN__)</span></div><div class='line' id='LC61'><span class="n">DACDriver</span> <span class="n">DACD1</span><span class="p">;</span></div><div class='line' id='LC62'><span class="cp">#endif</span></div><div class='line' id='LC63'><br/></div><div class='line' id='LC64'><span class="cm">/** @brief CHN2 driver identifier.*/</span></div><div class='line' id='LC65'><span class="cp">#if STM32_DAC_USE_CHN2 || defined(__DOXYGEN__)</span></div><div class='line' id='LC66'><span class="n">DACDriver</span> <span class="n">DACD2</span><span class="p">;</span></div><div class='line' id='LC67'><span class="cp">#endif</span></div><div class='line' id='LC68'><br/></div><div class='line' id='LC69'><span class="cm">/** @brief CHN3 driver identifier.*/</span></div><div class='line' id='LC70'><span class="cp">#if STM32_DAC_USE_CHN3 || defined(__DOXYGEN__)</span></div><div class='line' id='LC71'><span class="n">DACDriver</span> <span class="n">DACD3</span><span class="p">;</span></div><div class='line' id='LC72'><span class="cp">#endif</span></div><div class='line' id='LC73'><br/></div><div class='line' id='LC74'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC75'><span class="cm">/* Driver local variables.                                                   */</span></div><div class='line' id='LC76'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC77'><br/></div><div class='line' id='LC78'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC79'><span class="cm">/* Driver local functions.                                                   */</span></div><div class='line' id='LC80'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC81'><br/></div><div class='line' id='LC82'><br/></div><div class='line' id='LC83'><span class="cm">/**</span></div><div class='line' id='LC84'><span class="cm"> * @brief   Shared end/half-of-tx service routine.</span></div><div class='line' id='LC85'><span class="cm"> *</span></div><div class='line' id='LC86'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC87'><span class="cm"> * @param[in] flags     pre-shifted content of the ISR register</span></div><div class='line' id='LC88'><span class="cm"> */</span></div><div class='line' id='LC89'><span class="k">static</span> <span class="kt">void</span> <span class="nf">dac_lld_serve_tx_interrupt</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">,</span> <span class="kt">uint32_t</span> <span class="n">flags</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC90'><span class="cp">#if defined(STM32_DAC_DMA_ERROR_HOOK)</span></div><div class='line' id='LC91'>&nbsp;&nbsp;<span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="n">dacp</span><span class="p">;</span></div><div class='line' id='LC92'>&nbsp;&nbsp;<span class="k">if</span> <span class="p">((</span><span class="n">flags</span> <span class="o">&amp;</span> <span class="p">(</span><span class="n">STM32_DMA_ISR_TEIF</span> <span class="o">|</span> <span class="n">STM32_DMA_ISR_DMEIF</span><span class="p">))</span> <span class="o">!=</span> <span class="mi">0</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC93'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* DMA errors handling.*/</span></div><div class='line' id='LC94'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="c1">//~ _dac_isr_error_code(dacp, flags);</span></div><div class='line' id='LC95'>&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC96'>&nbsp;&nbsp;<span class="k">else</span> <span class="p">{</span></div><div class='line' id='LC97'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">((</span><span class="n">flags</span> <span class="o">&amp;</span> <span class="n">STM32_DMA_ISR_HTIF</span><span class="p">)</span> <span class="o">!=</span> <span class="mi">0</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC98'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* Half transfer processing.*/</span></div><div class='line' id='LC99'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="c1">//~ _dac_isr_half_code(dacp);</span></div><div class='line' id='LC100'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC101'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">((</span><span class="n">flags</span> <span class="o">&amp;</span> <span class="n">STM32_DMA_ISR_TCIF</span><span class="p">)</span> <span class="o">!=</span> <span class="mi">0</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC102'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* Transfer complete processing.*/</span></div><div class='line' id='LC103'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="c1">//~ _dac_isr_full_code(dacp);</span></div><div class='line' id='LC104'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC105'>&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC106'><span class="cp">#else</span></div><div class='line' id='LC107'>&nbsp;&nbsp;<span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="n">dacp</span><span class="p">;</span></div><div class='line' id='LC108'>&nbsp;&nbsp;<span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="n">flags</span><span class="p">;</span></div><div class='line' id='LC109'><span class="cp">#endif</span></div><div class='line' id='LC110'><span class="p">}</span></div><div class='line' id='LC111'><br/></div><div class='line' id='LC112'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC113'><span class="cm">/* Driver interrupt handlers.                                                */</span></div><div class='line' id='LC114'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC115'><br/></div><div class='line' id='LC116'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC117'><span class="cm">/* Driver exported functions.                                                */</span></div><div class='line' id='LC118'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC119'><br/></div><div class='line' id='LC120'><span class="cm">/**</span></div><div class='line' id='LC121'><span class="cm"> * @brief   Low level DAC driver initialization.</span></div><div class='line' id='LC122'><span class="cm"> *</span></div><div class='line' id='LC123'><span class="cm"> * @notapi</span></div><div class='line' id='LC124'><span class="cm"> */</span></div><div class='line' id='LC125'><span class="kt">void</span> <span class="nf">dac_lld_init</span><span class="p">(</span><span class="kt">void</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC126'><br/></div><div class='line' id='LC127'><span class="cp">#if STM32_DAC_USE_CHN1</span></div><div class='line' id='LC128'>&nbsp;&nbsp;<span class="n">dacObjectInit</span><span class="p">(</span><span class="o">&amp;</span><span class="n">DACD1</span><span class="p">);</span></div><div class='line' id='LC129'>&nbsp;&nbsp;<span class="n">DACD1</span><span class="p">.</span><span class="n">dac</span>     <span class="o">=</span> <span class="n">DAC1</span><span class="p">;</span></div><div class='line' id='LC130'>&nbsp;&nbsp;<span class="n">DACD1</span><span class="p">.</span><span class="n">tim</span>     <span class="o">=</span> <span class="n">STM32_TIM6</span><span class="p">;</span></div><div class='line' id='LC131'>&nbsp;&nbsp;<span class="n">DACD1</span><span class="p">.</span><span class="n">irqprio</span> <span class="o">=</span> <span class="n">STM32_DAC_CHN1_IRQ_PRIORITY</span><span class="p">;</span></div><div class='line' id='LC132'>&nbsp;&nbsp;<span class="n">DACD1</span><span class="p">.</span><span class="n">dma</span>     <span class="o">=</span> <span class="n">STM32_DMA_STREAM</span><span class="p">(</span><span class="n">STM32_DAC_CHN1_DMA_STREAM</span><span class="p">);</span></div><div class='line' id='LC133'>&nbsp;&nbsp;<span class="n">DACD1</span><span class="p">.</span><span class="n">dmamode</span> <span class="o">=</span> <span class="n">STM32_DMA_CR_CHSEL</span><span class="p">(</span><span class="n">DAC_CHN1_DMA_CHANNEL</span><span class="p">)</span> <span class="o">|</span>                  \</div><div class='line' id='LC134'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_PL</span><span class="p">(</span><span class="n">STM32_DAC_CHN1_DMA_PRIORITY</span><span class="p">)</span> <span class="o">|</span>              \</div><div class='line' id='LC135'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_DIR_M2P</span> <span class="o">|</span>                                      \</div><div class='line' id='LC136'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_DMEIE</span> <span class="o">|</span>                                        \</div><div class='line' id='LC137'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_TEIE</span> <span class="o">|</span>                                         \</div><div class='line' id='LC138'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_MINC</span> <span class="o">|</span> <span class="n">STM32_DMA_CR_TCIE</span><span class="p">;</span></div><div class='line' id='LC139'><span class="cp">#endif</span></div><div class='line' id='LC140'><br/></div><div class='line' id='LC141'><span class="cp">#if STM32_DAC_USE_CHN2</span></div><div class='line' id='LC142'>&nbsp;&nbsp;<span class="n">dacObjectInit</span><span class="p">(</span><span class="o">&amp;</span><span class="n">DACD2</span><span class="p">);</span></div><div class='line' id='LC143'>&nbsp;&nbsp;<span class="n">DACD2</span><span class="p">.</span><span class="n">dac</span>     <span class="o">=</span> <span class="n">DAC1</span><span class="p">;</span></div><div class='line' id='LC144'>&nbsp;&nbsp;<span class="n">DACD2</span><span class="p">.</span><span class="n">tim</span>     <span class="o">=</span> <span class="n">STM32_TIM7</span><span class="p">;</span></div><div class='line' id='LC145'>&nbsp;&nbsp;<span class="n">DACD2</span><span class="p">.</span><span class="n">irqprio</span> <span class="o">=</span> <span class="n">STM32_DAC_CHN2_IRQ_PRIORITY</span><span class="p">;</span></div><div class='line' id='LC146'>&nbsp;&nbsp;<span class="n">DACD2</span><span class="p">.</span><span class="n">dma</span>     <span class="o">=</span> <span class="n">STM32_DMA_STREAM</span><span class="p">(</span><span class="n">STM32_DAC_CHN2_DMA_STREAM</span><span class="p">);</span></div><div class='line' id='LC147'>&nbsp;&nbsp;<span class="n">DACD2</span><span class="p">.</span><span class="n">dmamode</span> <span class="o">=</span> <span class="n">STM32_DMA_CR_CHSEL</span><span class="p">(</span><span class="n">DAC_CHN2_DMA_CHANNEL</span><span class="p">)</span> <span class="o">|</span>                  \</div><div class='line' id='LC148'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_PL</span><span class="p">(</span><span class="n">STM32_DAC_CHN2_DMA_PRIORITY</span><span class="p">)</span> <span class="o">|</span>              \</div><div class='line' id='LC149'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_DIR_M2P</span> <span class="o">|</span>                                      \</div><div class='line' id='LC150'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_DMEIE</span> <span class="o">|</span>                                        \</div><div class='line' id='LC151'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_TEIE</span> <span class="o">|</span>                                         \</div><div class='line' id='LC152'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_MINC</span> <span class="o">|</span> <span class="n">STM32_DMA_CR_TCIE</span><span class="p">;</span></div><div class='line' id='LC153'><span class="cp">#endif</span></div><div class='line' id='LC154'><br/></div><div class='line' id='LC155'><span class="cp">#if STM32_DAC_USE_CHN3</span></div><div class='line' id='LC156'>&nbsp;&nbsp;<span class="n">dacObjectInit</span><span class="p">(</span><span class="o">&amp;</span><span class="n">DACD3</span><span class="p">);</span></div><div class='line' id='LC157'>&nbsp;&nbsp;<span class="n">DACD3</span><span class="p">.</span><span class="n">dac</span>     <span class="o">=</span> <span class="n">DAC2</span><span class="p">;</span></div><div class='line' id='LC158'>&nbsp;&nbsp;<span class="n">DACD3</span><span class="p">.</span><span class="n">tim</span>     <span class="o">=</span> <span class="n">STM32_TIM18</span><span class="p">;</span></div><div class='line' id='LC159'>&nbsp;&nbsp;<span class="n">DACD3</span><span class="p">.</span><span class="n">irqprio</span> <span class="o">=</span> <span class="n">STM32_DAC_CHN3_IRQ_PRIORITY</span><span class="p">;</span></div><div class='line' id='LC160'>&nbsp;&nbsp;<span class="n">DACD3</span><span class="p">.</span><span class="n">dma</span>     <span class="o">=</span> <span class="n">STM32_DMA_STREAM</span><span class="p">(</span><span class="n">STM32_DAC_CHN3_DMA_STREAM</span><span class="p">);</span></div><div class='line' id='LC161'>&nbsp;&nbsp;<span class="n">DACD3</span><span class="p">.</span><span class="n">dmamode</span> <span class="o">=</span> <span class="n">STM32_DMA_CR_CHSEL</span><span class="p">(</span><span class="n">DAC_CHN3_DMA_CHANNEL</span><span class="p">)</span> <span class="o">|</span>                  \</div><div class='line' id='LC162'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_PL</span><span class="p">(</span><span class="n">STM32_DAC_CHN2_DMA_PRIORITY</span><span class="p">)</span> <span class="o">|</span>              \</div><div class='line' id='LC163'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_DIR_M2P</span> <span class="o">|</span>                                      \</div><div class='line' id='LC164'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_DMEIE</span> <span class="o">|</span>                                        \</div><div class='line' id='LC165'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_TEIE</span> <span class="o">|</span>                                         \</div><div class='line' id='LC166'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_MINC</span> <span class="o">|</span> <span class="n">STM32_DMA_CR_TCIE</span><span class="p">;</span></div><div class='line' id='LC167'><span class="cp">#endif</span></div><div class='line' id='LC168'><span class="p">}</span></div><div class='line' id='LC169'><br/></div><div class='line' id='LC170'><span class="cm">/**</span></div><div class='line' id='LC171'><span class="cm"> * @brief   Configures and activates the DAC peripheral.</span></div><div class='line' id='LC172'><span class="cm"> *</span></div><div class='line' id='LC173'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC174'><span class="cm"> *</span></div><div class='line' id='LC175'><span class="cm"> * @notapi</span></div><div class='line' id='LC176'><span class="cm"> */</span></div><div class='line' id='LC177'><span class="kt">void</span> <span class="nf">dac_lld_start</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC178'>&nbsp;&nbsp;<span class="kt">uint32_t</span> <span class="n">arr</span><span class="p">,</span> <span class="n">regshift</span><span class="p">,</span> <span class="n">trgo</span><span class="p">,</span> <span class="n">dataoffset</span><span class="p">;</span></div><div class='line' id='LC179'>&nbsp;&nbsp;<span class="kt">bool</span> <span class="n">b</span><span class="p">;</span></div><div class='line' id='LC180'>&nbsp;&nbsp;<span class="cm">/* If in stopped state then enables the DAC and DMA clocks.*/</span></div><div class='line' id='LC181'>&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_STOP</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC182'><span class="cp">#if STM32_DAC_USE_CHN1</span></div><div class='line' id='LC183'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="o">&amp;</span><span class="n">DACD1</span> <span class="o">==</span> <span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC184'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">rccEnableDAC1</span><span class="p">(</span><span class="n">FALSE</span><span class="p">);</span></div><div class='line' id='LC185'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* DAC1 CR data is at bits 0:15 */</span></div><div class='line' id='LC186'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">regshift</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC187'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dataoffset</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC188'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* Timer setup */</span></div><div class='line' id='LC189'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">rccEnableTIM6</span><span class="p">(</span><span class="n">FALSE</span><span class="p">);</span></div><div class='line' id='LC190'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">rccResetTIM6</span><span class="p">();</span></div><div class='line' id='LC191'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">trgo</span> <span class="o">=</span> <span class="n">STM32_DAC_CR_TSEL_TIM6</span><span class="p">;</span></div><div class='line' id='LC192'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC193'><span class="cp">#endif</span></div><div class='line' id='LC194'><span class="cp">#if STM32_DAC_USE_CHN2</span></div><div class='line' id='LC195'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="o">&amp;</span><span class="n">DACD2</span> <span class="o">==</span> <span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC196'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">rccEnableDAC1</span><span class="p">(</span><span class="n">FALSE</span><span class="p">);</span></div><div class='line' id='LC197'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* DAC2 CR data is at bits 16:31 */</span></div><div class='line' id='LC198'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">regshift</span> <span class="o">=</span> <span class="mi">16</span><span class="p">;</span></div><div class='line' id='LC199'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dataoffset</span> <span class="o">=</span> <span class="o">&amp;</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">DHR12R2</span> <span class="o">-</span> <span class="o">&amp;</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">DHR12R1</span><span class="p">;</span></div><div class='line' id='LC200'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* Timer setup */</span></div><div class='line' id='LC201'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">rccEnableTIM7</span><span class="p">(</span><span class="n">FALSE</span><span class="p">);</span></div><div class='line' id='LC202'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">rccResetTIM7</span><span class="p">();</span></div><div class='line' id='LC203'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">trgo</span> <span class="o">=</span> <span class="n">STM32_DAC_CR_TSEL_TIM7</span><span class="p">;</span></div><div class='line' id='LC204'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC205'><span class="cp">#endif</span></div><div class='line' id='LC206'><span class="cp">#if STM32_DAC_USE_CHN3</span></div><div class='line' id='LC207'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="o">&amp;</span><span class="n">DACD3</span> <span class="o">==</span> <span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC208'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">rccEnableDAC2</span><span class="p">(</span><span class="n">FALSE</span><span class="p">);</span></div><div class='line' id='LC209'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* DAC3 CR data is at bits 0:15 */</span></div><div class='line' id='LC210'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">regshift</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC211'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dataoffset</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC212'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* Timer setup */</span></div><div class='line' id='LC213'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">rccEnableTIM18</span><span class="p">(</span><span class="n">FALSE</span><span class="p">);</span></div><div class='line' id='LC214'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">rccResetTIM18</span><span class="p">();</span></div><div class='line' id='LC215'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">trgo</span> <span class="o">=</span> <span class="n">STM32_DAC_CR_TSEL_TIM18</span><span class="p">;</span></div><div class='line' id='LC216'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC217'><span class="cp">#endif</span></div><div class='line' id='LC218'><span class="cp">#if STM32_DAC_USE_CHN1 || STM32_DAC_USE_CHN2 || STM32_DAC_USE_CHN3</span></div><div class='line' id='LC219'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">clock</span> <span class="o">=</span> <span class="n">STM32_TIMCLK1</span><span class="p">;</span></div><div class='line' id='LC220'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">arr</span> <span class="o">=</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">clock</span> <span class="o">/</span> <span class="n">dacp</span><span class="o">-&gt;</span><span class="n">config</span><span class="o">-&gt;</span><span class="n">frequency</span><span class="p">);</span></div><div class='line' id='LC221'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">osalDbgAssert</span><span class="p">((</span><span class="n">arr</span> <span class="o">&lt;=</span> <span class="mh">0xFFFF</span><span class="p">),</span></div><div class='line' id='LC222'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="s">&quot;invalid frequency&quot;</span><span class="p">);</span></div><div class='line' id='LC223'><br/></div><div class='line' id='LC224'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* Timer configuration.*/</span></div><div class='line' id='LC225'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">tim</span><span class="o">-&gt;</span><span class="n">CR1</span>  <span class="o">=</span> <span class="mi">0</span><span class="p">;</span>                        <span class="cm">/* Initially stopped.   */</span></div><div class='line' id='LC226'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">tim</span><span class="o">-&gt;</span><span class="n">PSC</span>  <span class="o">=</span> <span class="mi">0</span><span class="p">;</span>                        <span class="cm">/* Prescaler value.     */</span></div><div class='line' id='LC227'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">tim</span><span class="o">-&gt;</span><span class="n">DIER</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC228'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">tim</span><span class="o">-&gt;</span><span class="n">ARR</span>  <span class="o">=</span> <span class="n">arr</span><span class="p">;</span></div><div class='line' id='LC229'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">tim</span><span class="o">-&gt;</span><span class="n">EGR</span>  <span class="o">=</span> <span class="n">TIM_EGR_UG</span><span class="p">;</span>               <span class="cm">/* Update event.        */</span></div><div class='line' id='LC230'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">tim</span><span class="o">-&gt;</span><span class="n">CR2</span>  <span class="o">&amp;=</span> <span class="p">(</span><span class="kt">uint16_t</span><span class="p">)</span><span class="o">~</span><span class="n">TIM_CR2_MMS</span><span class="p">;</span></div><div class='line' id='LC231'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">tim</span><span class="o">-&gt;</span><span class="n">CR2</span>  <span class="o">|=</span> <span class="p">(</span><span class="kt">uint16_t</span><span class="p">)</span><span class="n">TIM_CR2_MMS_1</span><span class="p">;</span> <span class="cm">/* Enable TRGO updates. */</span></div><div class='line' id='LC232'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">tim</span><span class="o">-&gt;</span><span class="n">CNT</span>  <span class="o">=</span> <span class="mi">0</span><span class="p">;</span>                        <span class="cm">/* Reset counter.       */</span></div><div class='line' id='LC233'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">tim</span><span class="o">-&gt;</span><span class="n">SR</span>   <span class="o">=</span> <span class="mi">0</span><span class="p">;</span>                        <span class="cm">/* Clear pending IRQs.  */</span></div><div class='line' id='LC234'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* Update Event IRQ enabled. */</span></div><div class='line' id='LC235'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* Timer start.*/</span></div><div class='line' id='LC236'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">tim</span><span class="o">-&gt;</span><span class="n">CR1</span>  <span class="o">=</span> <span class="n">TIM_CR1_CEN</span><span class="p">;</span></div><div class='line' id='LC237'><br/></div><div class='line' id='LC238'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* DAC configuration */</span></div><div class='line' id='LC239'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">CR</span> <span class="o">|=</span> <span class="p">(</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">CR</span> <span class="o">&amp;</span> <span class="o">~</span><span class="n">STM32_DAC_CR_MASK</span><span class="p">)</span> <span class="o">|</span> \</div><div class='line' id='LC240'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">(</span><span class="n">STM32_DAC_CR_EN</span> <span class="o">|</span> <span class="n">STM32_DAC_CR_DMAEN</span> <span class="o">|</span> <span class="n">dacp</span><span class="o">-&gt;</span><span class="n">config</span><span class="o">-&gt;</span><span class="n">cr_flags</span><span class="p">)</span> <span class="p">)</span> <span class="o">&lt;&lt;</span> <span class="n">regshift</span><span class="p">;</span></div><div class='line' id='LC241'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</div><div class='line' id='LC242'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* DMA setup. */</span></div><div class='line' id='LC243'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">b</span> <span class="o">=</span> <span class="n">dmaStreamAllocate</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dma</span><span class="p">,</span></div><div class='line' id='LC244'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">irqprio</span><span class="p">,</span></div><div class='line' id='LC245'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">(</span><span class="kt">stm32_dmaisr_t</span><span class="p">)</span><span class="n">dac_lld_serve_tx_interrupt</span><span class="p">,</span></div><div class='line' id='LC246'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">(</span><span class="kt">void</span> <span class="o">*</span><span class="p">)</span><span class="n">dacp</span><span class="p">);</span></div><div class='line' id='LC247'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">osalDbgAssert</span><span class="p">(</span><span class="o">!</span><span class="n">b</span><span class="p">,</span> <span class="s">&quot;stream already allocated&quot;</span><span class="p">);</span></div><div class='line' id='LC248'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">switch</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">config</span><span class="o">-&gt;</span><span class="n">dhrm</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC249'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* Sets the DAC data register */</span></div><div class='line' id='LC250'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">case</span> <span class="n">DAC_DHRM_12BIT_RIGHT</span>:</div><div class='line' id='LC251'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dmaStreamSetPeripheral</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dma</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">DHR12R1</span> <span class="o">+</span> <span class="n">dataoffset</span><span class="p">);</span></div><div class='line' id='LC252'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">=</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">&amp;</span> <span class="o">~</span><span class="n">STM32_DMA_CR_SIZE_MASK</span><span class="p">)</span> <span class="o">|</span></div><div class='line' id='LC253'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_PSIZE_HWORD</span> <span class="o">|</span> <span class="n">STM32_DMA_CR_MSIZE_HWORD</span><span class="p">;</span></div><div class='line' id='LC254'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">break</span><span class="p">;</span></div><div class='line' id='LC255'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">case</span> <span class="n">DAC_DHRM_12BIT_LEFT</span>:</div><div class='line' id='LC256'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dmaStreamSetPeripheral</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dma</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">DHR12L1</span> <span class="o">+</span> <span class="n">dataoffset</span><span class="p">);</span></div><div class='line' id='LC257'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">=</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">&amp;</span> <span class="o">~</span><span class="n">STM32_DMA_CR_SIZE_MASK</span><span class="p">)</span> <span class="o">|</span></div><div class='line' id='LC258'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_PSIZE_HWORD</span> <span class="o">|</span> <span class="n">STM32_DMA_CR_MSIZE_HWORD</span><span class="p">;</span></div><div class='line' id='LC259'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">break</span><span class="p">;</span></div><div class='line' id='LC260'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">case</span> <span class="n">DAC_DHRM_8BIT_RIGHT</span>:</div><div class='line' id='LC261'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dmaStreamSetPeripheral</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dma</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">DHR8R1</span> <span class="o">+</span> <span class="n">dataoffset</span><span class="p">);</span></div><div class='line' id='LC262'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">=</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">&amp;</span> <span class="o">~</span><span class="n">STM32_DMA_CR_SIZE_MASK</span><span class="p">)</span> <span class="o">|</span></div><div class='line' id='LC263'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_PSIZE_BYTE</span> <span class="o">|</span> <span class="n">STM32_DMA_CR_MSIZE_BYTE</span><span class="p">;</span></div><div class='line' id='LC264'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">break</span><span class="p">;</span></div><div class='line' id='LC265'><span class="cp">#if defined(STM32_HAS_DAC_CHN2) &amp;&amp; STM32_HAS_DAC_CHN2</span></div><div class='line' id='LC266'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">case</span> <span class="n">DAC_DHRM_12BIT_RIGHT_DUAL</span>:</div><div class='line' id='LC267'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dmaStreamSetPeripheral</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dma</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">DHR12RD</span><span class="p">);</span></div><div class='line' id='LC268'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">=</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">&amp;</span> <span class="o">~</span><span class="n">STM32_DMA_CR_SIZE_MASK</span><span class="p">)</span> <span class="o">|</span></div><div class='line' id='LC269'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_PSIZE_HWORD</span> <span class="o">|</span> <span class="n">STM32_DMA_CR_MSIZE_HWORD</span><span class="p">;</span></div><div class='line' id='LC270'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">break</span><span class="p">;</span></div><div class='line' id='LC271'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">case</span> <span class="n">DAC_DHRM_12BIT_LEFT_DUAL</span>:</div><div class='line' id='LC272'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dmaStreamSetPeripheral</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dma</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">DHR12LD</span><span class="p">);</span></div><div class='line' id='LC273'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">=</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">&amp;</span> <span class="o">~</span><span class="n">STM32_DMA_CR_SIZE_MASK</span><span class="p">)</span> <span class="o">|</span></div><div class='line' id='LC274'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_PSIZE_HWORD</span> <span class="o">|</span> <span class="n">STM32_DMA_CR_MSIZE_HWORD</span><span class="p">;</span></div><div class='line' id='LC275'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">break</span><span class="p">;</span></div><div class='line' id='LC276'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">case</span> <span class="n">DAC_DHRM_8BIT_RIGHT_DUAL</span>:</div><div class='line' id='LC277'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dmaStreamSetPeripheral</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dma</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">DHR8RD</span><span class="p">);</span></div><div class='line' id='LC278'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">=</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">&amp;</span> <span class="o">~</span><span class="n">STM32_DMA_CR_SIZE_MASK</span><span class="p">)</span> <span class="o">|</span></div><div class='line' id='LC279'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">STM32_DMA_CR_PSIZE_BYTE</span> <span class="o">|</span> <span class="n">STM32_DMA_CR_MSIZE_BYTE</span><span class="p">;</span></div><div class='line' id='LC280'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">break</span><span class="p">;</span></div><div class='line' id='LC281'><span class="cp">#endif</span></div><div class='line' id='LC282'>&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC283'>&nbsp;&nbsp;</div><div class='line' id='LC284'>&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">CR</span> <span class="o">|=</span> <span class="n">trgo</span> <span class="o">&lt;&lt;</span> <span class="n">regshift</span><span class="p">;</span> <span class="cm">/* Enable timer trigger */</span></div><div class='line' id='LC285'><span class="cp">#endif</span></div><div class='line' id='LC286'>&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC287'><span class="p">}</span></div><div class='line' id='LC288'><br/></div><div class='line' id='LC289'><span class="cm">/**</span></div><div class='line' id='LC290'><span class="cm"> * @brief   Deactivates the DAC peripheral.</span></div><div class='line' id='LC291'><span class="cm"> *</span></div><div class='line' id='LC292'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC293'><span class="cm"> *</span></div><div class='line' id='LC294'><span class="cm"> * @notapi</span></div><div class='line' id='LC295'><span class="cm"> */</span></div><div class='line' id='LC296'><span class="kt">void</span> <span class="nf">dac_lld_stop</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC297'><br/></div><div class='line' id='LC298'>&nbsp;&nbsp;<span class="cm">/* If in ready state then disables the DAC clock.*/</span></div><div class='line' id='LC299'>&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">==</span> <span class="n">DAC_READY</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC300'><br/></div><div class='line' id='LC301'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* DMA disable.*/</span></div><div class='line' id='LC302'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dmaStreamRelease</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dma</span><span class="p">);</span></div><div class='line' id='LC303'><br/></div><div class='line' id='LC304'><span class="cp">#if STM32_DAC_USE_CHN1</span></div><div class='line' id='LC305'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="o">&amp;</span><span class="n">DACD1</span> <span class="o">==</span> <span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC306'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">CR</span> <span class="o">&amp;=</span> <span class="o">~</span><span class="n">STM32_DAC_CR_EN</span><span class="p">;</span> <span class="cm">/* DAC1 disable.*/</span></div><div class='line' id='LC307'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC308'><span class="cp">#endif</span></div><div class='line' id='LC309'><span class="cp">#if STM32_DAC_USE_CHN2</span></div><div class='line' id='LC310'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="o">&amp;</span><span class="n">DACD2</span> <span class="o">==</span> <span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC311'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">CR</span> <span class="o">&amp;=</span> <span class="o">~</span><span class="n">STM32_DAC_CR_EN</span> <span class="o">&lt;&lt;</span> <span class="mi">16</span><span class="p">;</span> <span class="cm">/* DAC1 disable.*/</span></div><div class='line' id='LC312'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC313'><span class="cp">#endif</span></div><div class='line' id='LC314'><span class="cp">#if STM32_DAC_USE_CHN3</span></div><div class='line' id='LC315'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="o">&amp;</span><span class="n">DACD3</span> <span class="o">==</span> <span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC316'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dac</span><span class="o">-&gt;</span><span class="n">CR</span> <span class="o">&amp;=</span> <span class="o">~</span><span class="n">STM32_DAC_CR_EN</span><span class="p">;</span> <span class="cm">/* DAC2 disable.*/</span></div><div class='line' id='LC317'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">rccDisableDAC2</span><span class="p">(</span><span class="n">FALSE</span><span class="p">);</span> <span class="cm">/* DAC Clock disable.*/</span></div><div class='line' id='LC318'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC319'><span class="cp">#endif</span></div><div class='line' id='LC320'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">tim</span><span class="o">-&gt;</span><span class="n">CR1</span> <span class="o">&amp;=</span> <span class="o">~</span><span class="n">TIM_CR1_CEN</span><span class="p">;</span> <span class="cm">/* Disable associated timer */</span></div><div class='line' id='LC321'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dacp</span><span class="o">-&gt;</span><span class="n">state</span> <span class="o">=</span> <span class="n">DAC_STOP</span><span class="p">;</span></div><div class='line' id='LC322'><br/></div><div class='line' id='LC323'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="o">!</span><span class="p">(</span><span class="n">DAC1</span><span class="o">-&gt;</span><span class="n">CR</span> <span class="o">&amp;</span> <span class="p">(</span><span class="n">STM32_DAC_CR_EN</span> <span class="o">|</span> <span class="n">STM32_DAC_CR_EN</span> <span class="o">&lt;&lt;</span> <span class="mi">16</span><span class="p">)))</span> <span class="p">{</span></div><div class='line' id='LC324'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* DAC Clock disable only if all channels are off.*/</span></div><div class='line' id='LC325'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">rccDisableDAC1</span><span class="p">(</span><span class="n">FALSE</span><span class="p">);</span></div><div class='line' id='LC326'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC327'>&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC328'><span class="p">}</span></div><div class='line' id='LC329'><br/></div><div class='line' id='LC330'><span class="cm">/**</span></div><div class='line' id='LC331'><span class="cm"> * @brief   Sends data over the DAC bus.</span></div><div class='line' id='LC332'><span class="cm"> * @details This asynchronous function starts a transmit operation.</span></div><div class='line' id='LC333'><span class="cm"> * @post    At the end of the operation the configured callback is invoked.</span></div><div class='line' id='LC334'><span class="cm"> *</span></div><div class='line' id='LC335'><span class="cm"> * @param[in] dacp      pointer to the @p DACDriver object</span></div><div class='line' id='LC336'><span class="cm"> * @param[in] n         number of words to send</span></div><div class='line' id='LC337'><span class="cm"> * @param[in] txbuf     the pointer to the transmit buffer</span></div><div class='line' id='LC338'><span class="cm"> *</span></div><div class='line' id='LC339'><span class="cm"> * @notapi</span></div><div class='line' id='LC340'><span class="cm"> */</span></div><div class='line' id='LC341'><span class="kt">void</span> <span class="nf">dac_lld_start_conversion</span><span class="p">(</span><span class="n">DACDriver</span> <span class="o">*</span><span class="n">dacp</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC342'>&nbsp;&nbsp;<span class="n">osalDbgAssert</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">samples</span><span class="p">,</span> </div><div class='line' id='LC343'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="s">&quot;dacp-&gt;samples is NULL pointer&quot;</span><span class="p">);</span></div><div class='line' id='LC344'>&nbsp;&nbsp;<span class="n">dmaStreamSetMemory0</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dma</span><span class="p">,</span> <span class="n">dacp</span><span class="o">-&gt;</span><span class="n">samples</span><span class="p">);</span></div><div class='line' id='LC345'>&nbsp;&nbsp;<span class="n">dmaStreamSetTransactionSize</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dma</span><span class="p">,</span> <span class="n">dacp</span><span class="o">-&gt;</span><span class="n">depth</span><span class="p">);</span></div><div class='line' id='LC346'>&nbsp;&nbsp;<span class="n">dmaStreamSetMode</span><span class="p">(</span><span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dma</span><span class="p">,</span> <span class="n">dacp</span><span class="o">-&gt;</span><span class="n">dmamode</span> <span class="o">|</span> <span class="n">STM32_DMA_CR_EN</span> <span class="o">|</span></div><div class='line' id='LC347'>&nbsp;&nbsp;<span class="n">STM32_DMA_CR_CIRC</span><span class="p">);</span></div><div class='line' id='LC348'><span class="p">}</span></div><div class='line' id='LC349'><span class="cp">#endif </span><span class="cm">/* HAL_USE_DAC */</span><span class="cp"></span></div><div class='line' id='LC350'><br/></div><div class='line' id='LC351'><span class="cm">/** @} */</span></div></pre></div></td>
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
      <li>&copy; 2014 <span title="0.07505s from github-fe132-cp1-prd.iad.github.net">GitHub</span>, Inc.</li>
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

