<template>
<div id='sidebar'>
    <section>
        <div class="sidebaritem">
            <h3><router-link to="/exchange">exchange</router-link> </h3>
        </div>
    </section>
   

</div>
</template>
<script>
export default {
  name: "sidebar"
}
</script>
<style scoped>
h3{
    color:#fbfbfb;
}
#sidebar {
    padding: 0;
    margin: 0;
    font-size: 12px;
    width: 210px;
    position: absolute;
    top: 60px;
    bottom: 0;
    left: 0;
    border-right: 1px solid #2d333b;
    background: #2c4155;
    display: flex;
    flex-direction: column;
    z-index: 20;
    color: #cfd0d2;
    box-shadow: 0 0 0 rgba(0,0,0,.5);
}
.sidebaritem {
    padding: 0;
    margin: 0;
    font-size: 12px;
    width: 210px;
    position: absolute;
    top: 60px;
    bottom: 0;
    left: 0;
    border-right: 1px solid #2d333b;
    background: #2c4155;
    display: flex;
    flex-direction: column;
    z-index: 20;
    color: #cfd0d2;
    box-shadow: 0 0 0 rgba(0,0,0,.5);
}

</style>